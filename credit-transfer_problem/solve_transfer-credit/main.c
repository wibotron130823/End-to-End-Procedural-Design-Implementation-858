#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// ===========================
//          DATA MOCKING
// ===========================

typedef struct {
    char nomor[20];
    long saldo;
} Akun;

Akun db_akun[2] = {
    {"081234567890", 100000}, // Akun Pengirim (Current Session)
    {"089876543210", 5000}    // Akun Penerima (Mock target)
};

typedef struct {
    char pengirim[20];
    char penerima[20];
    long nominal;
    char status[20];
} LogTransaksi;

LogTransaksi db_log[100];
int log_counter = 0;

// ==========================================
// PROCESS FUNCTIONS (Mewakili node pada DFD)
// ==========================================

// Menerima input awal dan mengaktifkan sesi
bool receive_input() {
    char ussd[20];
    int menu;

    printf("Dial UMB: ");
    scanf("%19s", ussd);

    if (strcmp(ussd, "*858#") == 0) {
        printf("\n--- MENU UMB ---\n");
        printf("1. Transfer Pulsa\n");
        printf("2. Minta Pulsa\n");
        printf("Pilih: ");
        scanf("%d", &menu);

        if (menu == 1) {
            return true; // Sesi aktif, lanjut ke tahap berikutnya
        } else {
            printf("[System] Pilihan menu tidak valid atau belum diimplementasi.\n");
            return false;
        }
    } else {
        printf("[System] Kode UMB tidak dikenal (Connection Problem or Invalid MMI Code).\n");
        return false;
    }
}

// Memvalidasi nomor tujuan
bool validate_goal_number(char* target_number) {
    printf("\nMasukkan nomor tujuan Transfer Pulsa: ");
    scanf("%19s", target_number);

    int len = strlen(target_number);
    // Validasi manual: panjang nomor dan prefix (misal harus diawali '08')
    if (len >= 10 && len <= 13 && target_number[0] == '0' && target_number[1] == '8') {
        return true;
    }
    printf("[System] Nomor tidak valid.\n");
    return false;
}

// Memvalidasi kecukupan nominal
bool validate_nominal(long* nominal_transfer, long saldo_pengirim) {
    printf("Masukkan nominal transfer: ");
    scanf("%ld", nominal_transfer);

    if (*nominal_transfer <= 0) {
        printf("[System] Nominal tidak valid.\n");
        return false;
    }

    if (saldo_pengirim >= *nominal_transfer) {
        return true; // Saldo cukup
    } else {
        printf("[System] Maaf, saldo Anda tidak mencukupi untuk melakukan transfer ini.\n");
        return false;
    }
}

// Konfirmasi akhir dari user
bool confirm(char* target_number, long nominal_transfer) {
    int konfirmasi;
    printf("\nHati-hati penipuan! Anda akan transfer pulsa Rp%ld ke %s.\n", nominal_transfer, target_number);
    printf("1. Ya\n");
    printf("0. Tidak\n");
    printf("Pilih: ");
    scanf("%d", &konfirmasi);

    return (konfirmasi == 1);
}

// Eksekusi transfer, update saldo, simpan log, dan kirim notifikasi
void execute_and_notify(char* target_number, long nominal_transfer) {
    // Cari index penerima di database (Linear Search sederhana)
    int target_idx = -1;
    for (int i = 0; i < 2; i++) {
        if (strcmp(db_akun[i].nomor, target_number) == 0) {
            target_idx = i;
            break;
        }
    }

    // Update Saldo (D2: db.saldo)
    db_akun[0].saldo -= nominal_transfer; // Potong saldo pengirim

    if (target_idx != -1) {
        db_akun[target_idx].saldo += nominal_transfer; // Tambah saldo penerima jika ada di DB
    }

    // Simpan Log (D3: db.log)
    strcpy(db_log[log_counter].pengirim, db_akun[0].nomor);
    strcpy(db_log[log_counter].penerima, target_number);
    db_log[log_counter].nominal = nominal_transfer;
    strcpy(db_log[log_counter].status, "SUKSES");
    log_counter++;

    // Notifikasi ke Layar/Pengirim
    printf("\n[System] Permintaan Anda sedang diproses. Terima kasih.\n");
    printf("--------------------------------------------------\n");
    printf("[SMS INBOX - %s]: Transfer pulsa senilai Rp%ld ke nomor %s SUKSES. Sisa saldo Anda: Rp%ld\n",
           db_akun[0].nomor, nominal_transfer, target_number, db_akun[0].saldo);

    // Notifikasi ke Penerima (Simulasi SMS Masuk)
    printf("[SMS INBOX - %s]: Anda menerima penambahan pulsa senilai Rp%ld dari %s\n",
           target_number, nominal_transfer, db_akun[0].nomor);
    printf("--------------------------------------------------\n");
}

// ==========================================
//              MAIN FUNCTION
// ==========================================
int main() {
    char nomor_tujuan[20];
    long nominal_transfer = 0;

    printf("=== SIMULASI LAYANAN *858# ===\n");
    printf("Saldo Anda saat ini: Rp%ld\n\n", db_akun[0].saldo);

    if (receive_input()) {
        if (validate_goal_number(nomor_tujuan)) {
            if (validate_nominal(&nominal_transfer, db_akun[0].saldo)) {
                if (confirm(nomor_tujuan, nominal_transfer)) {
                    execute_and_notify(nomor_tujuan, nominal_transfer);
                } else {
                    printf("[System] Transaksi dibatalkan oleh pengguna.\n");
                }
            }
        }
    }

    return 0;
}
