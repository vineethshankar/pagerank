#include <stdio.h>
#include <string.h>
main() {
	FILE *fp;
	char buf[1000];
	char nodes[10], edges[15];
	int sub_index = 1;
	fp = fopen("sample2.txt", "r");
	fgets(buf,1000, fp);
	puts(*(buf+3));
//	printf("%d", atoi(buf[9]));
//	while (!(strcmp(buf[9+sub_index]," "))) {
//			nodes[sub_index] = buf[9+sub_index];
//			}
	fclose(fp);
//	printf("number of nodes is %d\n", nodes);
//	printf("number of edges is %d\n", edges);
}

