#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define n 101
int kolvo_blya(char* slovo)
{
	char *kk, *pp=NULL, symbols[] = "blya";
	int k_bya=0;
	while (1)
	{
		kk = strstr (slovo, symbols);
		if (kk==NULL)
		{
			if (k_bya==1)
			{
				slovo[pp - slovo]--;
			}
			break;
		}
		else
		{
			k_bya++;
			pp = kk;
			slovo[kk - slovo]++;
		}
	}
	return k_bya;
}
void push_back_zvezd(char* correct, int* i_corr)
{
	correct[*i_corr] = '*';
	*i_corr+=1;
	correct[*i_corr] = '*';
	*i_corr+=1;
	correct[*i_corr] = '*';
	*i_corr+=1;
}
int main ()
{
	int sockfd, connfd; 
    struct sockaddr_in servaddr, cli; 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) 
	{ 
        printf("socket creation failed...\n"); 
        return -1; 
    }
    bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("188.166.172.250");
    servaddr.sin_port = htons(4041);
	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0)
	{ 
        printf("connection with the server failed...\n");
        return -1;
    }
	char fraza[n], correct[n], slovo[n], symbols[] = "blya", flag[]="flag";
	//fgets (fraza, n-1, stdin);
	while (1)
	{
	read(sockfd, fraza, sizeof(fraza));
	char*ll;
	ll=strstr(fraza,flag);
	if (ll!=NULL)
	{
		printf("%s", fraza);
		break;
	}
	int i = 2, i_slovo=0, i_corr=0;
	while (fraza[i]!='\n')
	{
		if (!((fraza[i]>='a' && fraza[i] <= 'z')||(fraza[i] >= 'A' && fraza[i] <= 'Z')))
		{
			correct[i_corr] = fraza[i];
			i_corr++;
			i++;
			continue;
		}
		slovo[i_slovo] = fraza[i];
		if (!((fraza[i+1] >= 'a' && fraza[i+1] <= 'z') || (fraza[i+1] >= 'A' && fraza[i+1] <= 'Z')))
		{
			int flag_fck = 0;
			if ((slovo[0]=='f' || slovo[0] == 'F') && (slovo[i_slovo]=='k' || slovo[i_slovo] == 'K') && (slovo[i_slovo-1] == 'c' || slovo[i_slovo - 1] == 'C'))
			{
				flag_fck = 1;
			}
			int j = 0;			
			slovo[i_slovo + 1] = 0;
			int k_bya=kolvo_blya(slovo);
			if (flag_fck == 1 && k_bya == 0)
			{
				correct[i_corr] = slovo[0];
				i_corr++;
				push_back_zvezd(correct, &i_corr);
			}
			if (flag_fck == 0 && k_bya == 1)
			{
				char* kk;
				kk = strstr (slovo, symbols);				
				j = 0;
				while (j <= i_slovo)
				{
					if (j==(kk-slovo))
					{
						push_back_zvezd(correct, &i_corr);
						j += 4;
						continue;
					}
					correct[i_corr] = slovo[j];
					i_corr++;
					j++;
				}
			}
			if (flag_fck==1 && k_bya>0)
			{
				push_back_zvezd(correct, &i_corr);
			}
			if (flag_fck == 0 && k_bya >1)
			{
				int k = k_bya;
				while (k)
				{
					push_back_zvezd(correct, &i_corr);
					k--;
				}
			}
			if (flag_fck == 0 && k_bya == 0)
			{
				j = 0;
				while (j <= i_slovo)
				{
					correct[i_corr] = slovo[j];
					i_corr++;
					j++;
				}
			}
			i_slovo = -1;
		}
		i_slovo++;
		i++;
	}
	correct[i_corr] = '\n';
	correct[i_corr +1] = 0;
	write(sockfd, correct, i_corr);
	printf ("%s", correct);
	char bot_write[n];
	read(sockfd, bot_write, sizeof(bot_write));
	printf("%s",bot_write);
	}
	return 0;
}