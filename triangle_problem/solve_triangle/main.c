#include <stdio.h>
#include <stdbool.h>

double getAbsolute(double val) {
    if (val < 0) {
        return -val;
    }
    return val;
}

void swapDouble(double *a, double *b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

bool isEqual(double a, double b) {
    double selisih = getAbsolute(a - b);
    double nilaiTerbesar;

    if (getAbsolute(a) > getAbsolute(b)) {
        nilaiTerbesar = getAbsolute(a);
    } else {
        nilaiTerbesar = getAbsolute(b);
    }

    if (nilaiTerbesar == 0.0) {
        return true;
    }

    return ((selisih / nilaiTerbesar) <= 0.01);

}


void determineTriangle(double a, double b, double c) {

    if (a <= 0 || b <= 0 || c <= 0) {
        printf("Input (%.2f, %.2f, %.2f): Tidak ada segitiga dapat dibangun.\n", a, b, c);
        return;
    }

    double x = a, y = b, z = c;
    if (x > y) {
        swapDouble(&x, &y);
    }
    if (y > z) {
        swapDouble(&y, &z);
    }
    if (x > y) {
        swapDouble(&x, &y);
    }

    if (z > (x + y) || isEqual(z, x + y)) {
        printf("Input (%.2f, %.2f, %.2f): Tidak ada segitiga dapat dibangun.\n", a, b, c);
        return;
    }

    if (isEqual(a, b) && isEqual(b, c)) {
        printf("Input (%.2f, %.2f, %.2f): Segitiga SAMA SISI (EQUILATERAL)\n", a, b, c);
    }
    else if (isEqual(a, b) || isEqual(b, c) || isEqual(a, c)) {
        printf("Input (%.2f, %.2f, %.2f): Segitiga SAMA KAKI (ISOSCELES)\n", a, b, c);
    }
    else if (isEqual(z * z, (x * x) + (y * y))) {
        printf("Input (%.2f, %.2f, %.2f): Segitiga SIKU-SIKU (RIGHT TRIANGLE)\n", a, b, c);
    }
    else {
        printf("Input (%.2f, %.2f, %.2f): Segitiga BEBAS\n", a, b, c);
    }

}

int main() {
    printf("=== HASIL EKSEKUSI PROGRAM SEGITIGA ===\n");

    // Test Case 1: Bilangan Bulat (siku-siku)
    determineTriangle(3, 4, 5);

    // Test Case 2: sama sisi
    determineTriangle(10, 10, 10);

    // Test Case 3: Pecahan dengan toleransi 1% (5.0 dan 5.04 dianggap sama)
    // 0.04 adalah 0.8% dari 5.0, seharusnya dianggap SAMA KAKI
    determineTriangle(5.0, 5.0, 5.04);

    // Test Case 4: Bukan segitiga
    determineTriangle(1, 2, 10);

    return 0;
}
