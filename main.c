#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define STRING_SIZE 100

#define ullong unsigned long long int
#define uint unsigned int
#define uldouble long double
#define ERROR 0.0000000000001

char * generateString(uint size) {
    char * str = (char *)malloc(size * sizeof(char));
    for (uint i = 0; i < size; i++) {
        str[i] = '\0';
    }
    // str[size - 1] = '\0';
    return str;
}

uint numfrmch(char num) {
    if ((uint)num <= (uint)'9')
        return (uint)num - '0';

    if ((uint)num <= (uint)'Z')
        return (uint)num - 'A' + 10;

    if ((uint)num <= (uint)'z')
        return (uint)num - 'a' + 10;

    return 0;
}

char charfrnum(ullong num) {
    if (num < 10)
        return num + '0';

    return num - 10 + 'A';
}

uint validatei1(uint b1, uint b2) {
    if (b1 > 16 || b2 > 16 || b1 < 2 || b2 < 2)
        return 0;
    return 1;
}

uint validatei2(char * X, uint b1) {
    for (uint i = 0; i < strlen(X); i++) {
        int code = (int)X[i];

        if (code == '.')
            continue;
        
        if (tolower(code) >= 'a' && code >= tolower(charfrnum(b1)))
            return 0;
        else if (code <= '9' && code >= charfrnum(b1))
            return 0;
    }
    return 1;
}

uint validaten(char * X) {
    uint dotCount = 0;
    for (uint i = 0; i < strlen(X); i++) {
        int code = (int)X[i];

        if ((code < '0' || code > '9') &&
            (code < 'A' || code > 'Z') &&
            (code < 'a' || code > 'z') &&
            code != '.')
            return 0;

        if (code == '.') {
            dotCount++;
            if (i >= strlen(X) - 1 || i == 0)
                return 0;
        }

        if (dotCount > 1)
            return 0;
    }
    return 1;
}

uint validatei(char * X, uint b1, uint b2) {
    uint v1 = validatei1(b1, b2);
    uint v2 = validatei2(X, b1);
    uint v3 = validaten(X);

    return v1 && v2 && v3;
}

uldouble cnv10(char * X, uint b1, int fractional) {
    uldouble result = 0;
    int step = fractional ? -1 : (int)(strlen(X) - 1);

    for (uint i = 0; i < strlen(X); i++) {
        if (X[i] != ' ') {
            result += numfrmch(X[i]) * pow(b1, step);
            step += -1;
        }
    }
    return result;
}

char * cnvn(uldouble num, uint base) {
    ullong whole = (ullong)num;
    uldouble fractional = num - whole;
    char * wholeres = generateString(STRING_SIZE);
    if (!wholeres)
        exit(0);

    uint index = 0;

    while (whole >= base) {
        wholeres[index] = charfrnum(whole % base);
        index++;
        whole /= base;
    }
    wholeres[index] = charfrnum(whole % base);
    wholeres[index + 1] = '\0';


    uint wholelen = strlen(wholeres);

    char * result = generateString(STRING_SIZE);
    if (!result) {
        free(wholeres);
        exit(0);
    }

    for (uint i = 0; i < wholelen; i++) {
        result[i] = wholeres[wholelen - i - 1];
    }

    free(wholeres);

    if (fractional == 0.0)
        return result;
    
    result[index + 1] = '.';
    index += 2;

    while (fractional > 0) {
        // pruintf("%llf - test\n", fractional);
        
        fractional *= base;
        uint digit = (uint)(fractional + ERROR);
        result[index] = charfrnum(digit);
        fractional -= digit;
        index++;
    }

    return result;
}

char * cnvrn(char * X, uint b1, uint b2) {
    char * whole = strtok(X, ".");

    uldouble num10 = cnv10(whole, b1, 0);

    char * fractional = strtok(NULL, ".");

    if (fractional != NULL)
        num10 += cnv10(fractional, b1, 1);
    
    free(X);

    return cnvn(num10, b2);
}

int main() {
    ullong b1;
    ullong b2;
    char * X = generateString(STRING_SIZE);
    if (!X)
        return 1;

    if (!scanf("%llu %llu\n", &b1, &b2)) {
        free(X);
        return 0;
    }
    if (!fgets(X, STRING_SIZE, stdin)) {
        free(X);
        return 0;
    }

    X[strlen(X) - 1] = '\0';

    if (!validatei(X, (uint)b1, (uint)b2)) {
        printf("bad input");
        free(X);
        return 0;
    }

    char * result = cnvrn(X, (uint)b1, (uint)b2);
    puts(result);

    free(result);

    return 0;
}