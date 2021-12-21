#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> 

int arr[10], s = 0;
int res[] = {97, 241, 1797, 1959, 1542, 1632, 1765, 1283, 1479, 1496, 91, 145, 1821, 1971, 1558, 1643, 1751, 1385, 1500, 1518, 52, 238, 1806, 1999, 1599, 1591, 1774, 1288, 1428, 1522, 95, 206, 1910, 1933, 1580, 1577, 1751, 1301, 1455, 1483, 126, 241, 1847, 1958, 1580, 1621, 1790, 1286, 1441, 1467, 105, 156, 1806, 1972, 1595, 1652, 1744, 1329, 1419, 1468, 126, 237, 1847, 1943, 1547, 1658, 1665, 1312, 1455, 1478, 97, 135, 1836, 1969, 1546, 1591, 1751, 1343, 1440, 1490, 120, 219, 1797, 1938, 1541, 1663, 1774, 1383, 1441, 1467, 117, 145, 1814, 1971, 1572, 1660, 1744, 1286, 1419, 1522, 122, 225, 1904, 1968, 1540, 1587, 1665, 1321, 1468, 1478, 71, 198, 1907, 1934, 1611, 1649, 1677, 1314, 1418, 1474, 121, 225, 1813, 1971, 1592, 1663, 1673, 1389};
const char chars[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+/";

// 892
// Give me something to check: Wanna.One{https://www.youtube.com/watch?v=pDxv3pl9U3Q_;_Kaguya_is_the_best_fucking_waifu_ever}
// mYAGzxQS+PW9YSjsc90f8FJ/C/ZXxzSf2mP1cECCrYsFPMJVM3e4JTGldag4rEswwb5pCNm/hQv/coLZtsArygZ7M3y9RSXddVgzvI4Px+5yPNKn7n7i9rfJuIQSDg==

void __attribute__((constructor)) something()
{
    srand(time(NULL));
    int random_val = rand() % 1000;
    // printf("%d\n", random_val);
    // random_val = 892;

    for (int i=0; i<10; i++)
    	arr[i] = (i * 100 - 12 + random_val) ^ random_val;
}

size_t another_sus_function(size_t inlen)
{
	size_t ret;

	ret = inlen;
	if (inlen % 3 != 0)
		ret += 3 - (inlen % 3);
	ret /= 3;
	ret *= 4;

	return ret;
}

char *sus_function(const unsigned char *in, size_t len)
{
	char   *out;
	size_t  elen;
	size_t  i;
	size_t  j;
	size_t  v;

	if (in == NULL || len == 0)
		return NULL;

	elen = another_sus_function(len);
	out  = malloc(elen+1);
	out[elen] = '\0';

	for (i=0, j=0; i<len; i+=3, j+=4) {
		v = in[i];
		v = i+1 < len ? v << 8 | in[i+1] : v << 8;
		v = i+2 < len ? v << 8 | in[i+2] : v << 8;

		out[j]   = chars[(v >> 18) & 0x3F];
		out[j+1] = chars[(v >> 12) & 0x3F];
		if (i+1 < len) {
			out[j+2] = chars[(v >> 6) & 0x3F];
		} else {
			out[j+2] = '=';
		}
		if (i+2 < len) {
			out[j+3] = chars[v & 0x3F];
		} else {
			out[j+3] = '=';
		}
	}

	return out;
}

int main() {
	char a[100];
	char *out;
	int stat;
	printf("Give me something to check: ");
	scanf("%100s", a);
	// puts(a);
	
	__pid_t pid = fork();

	if (pid < 0) {
       printf("Something's wrong");
       return 1;
   }

	if (pid) { 	// Parent
		waitpid(pid, &stat, 0);
      	if (WEXITSTATUS(stat) == 2) {
      		printf("What a failure!!\n");
      	}
      	else printf("You're so lucky with that timing my dude!!\n");
    }
	else {	// Child
		int le = strlen(a);
		for (int i=0; i<le; i++)
			a[i] = (char)((int)a[i] ^ arr[i % 10]);
		out = sus_function(a, le);
		// puts(out);
		
		for (int i=0; i<128; i++) {
			if ((int)(out[i] ^ arr[i % 10]) != res[i]) exit(2);
			// printf("%d, ", (int)(out[i] ^ arr[i % 10]));
		}
		// putchar(10);
		exit(3);
	}

	return 0;
}