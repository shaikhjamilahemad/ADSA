#include <stdio.h>
#include <string.h>
#include <math.h>

#define d 256       
#define q 101       


void computeLPSArray(char* pat, int M, int* lps) {
    int len = 0;
    lps[0] = 0;
    int i = 1;

    while (i < M) {
        if (pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0)
                len = lps[len - 1];
            else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

void KMP_Search(char* pat, char* txt) {
    int M = strlen(pat);
    int N = strlen(txt);
    int lps[M];
    computeLPSArray(pat, M, lps);

    int i = 0; 
    int j = 0; 
    int found = 0;

    printf("\n--- Knuth-Morris-Pratt (KMP) Algorithm ---\n");
    while (i < N) {
        if (pat[j] == txt[i]) {
            j++;
            i++;
        }

        if (j == M) {
            printf("Pattern found at index %d\n", i - j);
            found = 1;
            j = lps[j - 1];
        } else if (i < N && pat[j] != txt[i]) {
            if (j != 0)
                j = lps[j - 1];
            else
                i++;
        }
    }

    if (!found)
        printf("Pattern not found in text.\n");
}


void RabinKarp_Search(char* pat, char* txt) {
    int M = strlen(pat);
    int N = strlen(txt);
    int i, j;
    int p = 0;  
    int t = 0;  
    int h = 1;
    int found = 0;

    
    for (i = 0; i < M - 1; i++)
        h = (h * d) % q;

    
    for (i = 0; i < M; i++) {
        p = (d * p + pat[i]) % q;
        t = (d * t + txt[i]) % q;
    }

    printf("\n--- Rabin-Karp (Fingerprinting) Algorithm ---\n");
    
    for (i = 0; i <= N - M; i++) {
        if (p == t) {
            
            for (j = 0; j < M; j++) {
                if (txt[i + j] != pat[j])
                    break;
            }
            if (j == M) {
                printf("Pattern found at index %d\n", i);
                found = 1;
            }
        }

        
        if (i < N - M) {
            t = (d * (t - txt[i] * h) + txt[i + M]) % q;
            if (t < 0)
                t = (t + q);
        }
    }

    if (!found)
        printf("Pattern not found in text.\n");
}


int main() {
    char txt[200], pat[100];

    printf("Enter the text string: ");
    gets(txt); 
    printf("Enter the pattern to search: ");
    gets(pat);

    KMP_Search(pat, txt);
    RabinKarp_Search(pat, txt);

    return 0;
}
