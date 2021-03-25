#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

char address[100];
char outcome;
char bin[100]="";
char new_bin[100]="";
char gshare_final_bin[100]="";
char bimodal_final_bin[100]="";
char chooser_final_bin[100]="";
int predictions=0;
int misspredictions=0;

void conv_hex_to_bin(){
	int i;
	for(i=0; address[i]!='\0'; i++)
    {
        switch(address[i])
        {
            case '0':
                strcat(bin, "0000");
                break;
            case '1':
                strcat(bin, "0001");
                break;
            case '2':
                strcat(bin, "0010");
                break;
            case '3':
                strcat(bin, "0011");
                break;
            case '4':
                strcat(bin, "0100");
                break;
            case '5':
                strcat(bin, "0101");
                break;
            case '6':
                strcat(bin, "0110");
                break;
            case '7':
                strcat(bin, "0111");
                break;
            case '8':
                strcat(bin, "1000");
                break;
            case '9':
                strcat(bin, "1001");
                break;
            case 'a':
            case 'A':
                strcat(bin, "1010");
                break;
            case 'b':
            case 'B':
                strcat(bin, "1011");
                break;
            case 'c':
            case 'C':
                strcat(bin, "1100");
                break;
            case 'd':
            case 'D':
                strcat(bin, "1101");
                break;
            case 'e':
            case 'E':
                strcat(bin, "1110");
                break;
            case 'f':
            case 'F':
                strcat(bin, "1111");
                break;
            default:
                printf("Invalid hexadecimal input.");
        }
    }
}

int main(int argc, char *argv[]){
	
    char predictor[10];
    strcpy(predictor, argv[1]);

    int bhr=0;
	int i,j;
	int multiplier=0;
	int dec=0;
    int m_gshare,m_bimodal,n,k,gshare_pred_table_size,bimodal_pred_table_size,chooser_table_size;
    

    int *gshare_pred_table = (int *) malloc(1048576 * sizeof(int));
    int *bimodal_pred_table = (int *) malloc(1048576 * sizeof(int));
    int *chooser_table = (int *) malloc(1048576 * sizeof(int));

    if(strncmp(predictor, "gshare", 6)==0){
        m_gshare=atoi(argv[2]);
        n=atoi(argv[3]);
        gshare_pred_table_size=pow(2, m_gshare);
        realloc(gshare_pred_table, gshare_pred_table_size);
    }else if(strncmp(predictor, "bimodal", 7)==0){
        m_bimodal=atoi(argv[2]);
        bimodal_pred_table_size=pow(2, m_bimodal);
        realloc(bimodal_pred_table, bimodal_pred_table_size);
    }else if(strncmp(predictor, "hybrid", 6)==0){
        k=atoi(argv[2]);
        m_gshare=atoi(argv[3]);
        n=atoi(argv[4]);
        m_bimodal=atoi(argv[5]);
        gshare_pred_table_size=pow(2, m_gshare);
        realloc(gshare_pred_table, gshare_pred_table_size);
        bimodal_pred_table_size=pow(2, m_bimodal);
        realloc(bimodal_pred_table, bimodal_pred_table_size);
        chooser_table_size=pow(2, k);
        realloc(chooser_table, chooser_table_size);
    }

	
	int bimodal_index;
    int gshare_index;
    int chooser_index;
    int type;

    if(strncmp(predictor, "gshare", 6)==0){
        type=1;
    }else if(strncmp(predictor, "bimodal", 7)==0){
        type=0;
    }else if(strncmp(predictor, "hybrid", 6)==0){
        type=2;
    }

    if(type==0){
        for(i=0; i<bimodal_pred_table_size; i++){
           bimodal_pred_table[i]=4;
        }
    }else if(type==1){
        for(i=0; i<gshare_pred_table_size; i++){
           gshare_pred_table[i]=4;
        }
    }else if(type==2){
        for(i=0; i<bimodal_pred_table_size; i++){
           bimodal_pred_table[i]=4;
        }
        for(i=0; i<gshare_pred_table_size; i++){
           gshare_pred_table[i]=4;
        }
        for(i=0; i<chooser_table_size; i++){
           chooser_table[i]=1;
        }
    }

	FILE *fp;
    if(strncmp(predictor, "gshare", 6)==0){
        fp=fopen(argv[4],"r");
    }else if(strncmp(predictor, "bimodal", 7)==0){
        fp=fopen(argv[3],"r");
    }else if(strncmp(predictor, "hybrid", 6)==0){
        fp=fopen(argv[6],"r");
    }
    
    fseek(fp,0,SEEK_SET);
    while(fscanf(fp,"%s %c",address, &outcome))
    {	
    	multiplier=0;
		int gshare_dec=0;
        int bimodal_dec=0;
        int chooser_dec=0;
        if(feof(fp))
        {	
            break;
        }
        conv_hex_to_bin();
		predictions++;
        for(i=0; i<22; i++){
	    	new_bin[i]=bin[i];
	    }

        if(type==0){
            for(i=22-m_bimodal, j=0; i<22; i++, j++){
                bimodal_final_bin[j]=new_bin[i];
            }

            for(i=m_bimodal-1; i>=0; i--){
                if(bimodal_final_bin[i]=='0'){
                    bimodal_dec=bimodal_dec+(0*pow(2, multiplier));
                }else{
                    bimodal_dec=bimodal_dec+(1*pow(2, multiplier));
                }
                multiplier++;
            }    
        }else if(type==1){
            for(i=22-m_gshare, j=0; i<22; i++, j++){
                gshare_final_bin[j]=new_bin[i];
            }

            for(i=m_gshare-1; i>=0; i--){
                if(gshare_final_bin[i]=='0'){
                    gshare_dec=gshare_dec+(0*pow(2, multiplier));
                }else{
                    gshare_dec=gshare_dec+(1*pow(2, multiplier));
                }
                multiplier++;
            }
        }else{
            for(i=22-m_bimodal, j=0; i<22; i++, j++){
                bimodal_final_bin[j]=new_bin[i];
            }

            for(i=m_bimodal-1; i>=0; i--){
                if(bimodal_final_bin[i]=='0'){
                    bimodal_dec=bimodal_dec+(0*pow(2, multiplier));
                }else{
                    bimodal_dec=bimodal_dec+(1*pow(2, multiplier));
                }
                multiplier++;
            }

            multiplier=0;

            for(i=22-m_gshare, j=0; i<22; i++, j++){
                gshare_final_bin[j]=new_bin[i];
            }

            for(i=m_gshare-1; i>=0; i--){
                if(gshare_final_bin[i]=='0'){
                    gshare_dec=gshare_dec+(0*pow(2, multiplier));
                }else{
                    gshare_dec=gshare_dec+(1*pow(2, multiplier));
                }
                multiplier++;
            }

            multiplier=0;

            for(i=22-k, j=0; i<22; i++, j++){
                chooser_final_bin[j]=new_bin[i];
            }

            for(i=k-1; i>=0; i--){
                if(chooser_final_bin[i]=='0'){
                    chooser_dec=chooser_dec+(0*pow(2, multiplier));
                }else{
                    chooser_dec=chooser_dec+(1*pow(2, multiplier));
                }
                multiplier++;
            }            
        }

	    if(strncmp(predictor, "gshare", 6)==0){
	    	gshare_index=gshare_dec^bhr;
	    }else if(strncmp(predictor, "bimodal", 7)==0){
	    	bimodal_index=bimodal_dec;
	    }else if(strncmp(predictor, "hybrid", 6)==0){
            bimodal_index=bimodal_dec;
            gshare_index=gshare_dec^bhr;
            chooser_index=chooser_dec;
        }

        char predicted_outcome;
        int predictor_used;

        if(type==2){
            if(chooser_table[chooser_index]<=1 && bimodal_pred_table[bimodal_index]<4){
                predicted_outcome='n';
                predictor_used=0;
            }else if(chooser_table[chooser_index]<=1 && bimodal_pred_table[bimodal_index]>=4){
                predicted_outcome='t';
                predictor_used=0;
            }else if(chooser_table[chooser_index]>1 && gshare_pred_table[gshare_index]<4){
                predicted_outcome='n';
                predictor_used=1;
            }else if(chooser_table[chooser_index]>1 && gshare_pred_table[gshare_index]>=4){
                predicted_outcome='t';
                predictor_used=1;
            }    
        }
        

	    if(outcome=='t'){
            if(type==0){
                if(bimodal_pred_table[bimodal_index]<4){
                    misspredictions++;
                }    
            }else if(type==1){
                if(gshare_pred_table[gshare_index]<4){
                    misspredictions++;
                }
            }else if(type==2){
                if(predicted_outcome!=outcome){
                    misspredictions++;
                }
            }
	    }else if(outcome=='n'){
            if(type==0){
                if(bimodal_pred_table[bimodal_index]>3){
                    misspredictions++;
                }    
            }else if(type==1){
                if(gshare_pred_table[gshare_index]>3){
                    misspredictions++;
                }
            }else if(type==2){
                if(predicted_outcome!=outcome){
                    misspredictions++;
                }
            }
	    }	    

        

        if(type==0){
            if(bimodal_pred_table[bimodal_index]>0 && outcome=='n'){
                bimodal_pred_table[bimodal_index]--;
            }else if(bimodal_pred_table[bimodal_index]<7 && outcome=='t'){
                bimodal_pred_table[bimodal_index]++;
            }
        }else if(type==1){
            if(gshare_pred_table[gshare_index]>0 && outcome=='n'){
                gshare_pred_table[gshare_index]--;
            }else if(gshare_pred_table[gshare_index]<7 && outcome=='t'){
                gshare_pred_table[gshare_index]++;
            }
        }else if(type==2){

            if(outcome=='t'){
                if(bimodal_pred_table[bimodal_index]>3 && gshare_pred_table[gshare_index]<=3 && chooser_table[chooser_index]>0){
                    chooser_table[chooser_index]--;
                }else if(bimodal_pred_table[bimodal_index]<=3 && gshare_pred_table[gshare_index]>3 && chooser_table[chooser_index]<3){
                    chooser_table[chooser_index]++;
                }
            }

            if(outcome=='n'){
                if(bimodal_pred_table[bimodal_index]>3 && gshare_pred_table[gshare_index]<=3 && chooser_table[chooser_index]<3){
                    chooser_table[chooser_index]++;
                }else if(bimodal_pred_table[bimodal_index]<=3 && gshare_pred_table[gshare_index]>3 && chooser_table[chooser_index]>0){
                    chooser_table[chooser_index]--;
                }
            }

            if(predictor_used==0 && outcome=='t' && bimodal_pred_table[bimodal_index]<7){
                bimodal_pred_table[bimodal_index]++;
            }else if(predictor_used==0 && outcome=='n' && bimodal_pred_table[bimodal_index]>0){
                bimodal_pred_table[bimodal_index]--;
            }else if(predictor_used==1 && outcome=='t' && gshare_pred_table[gshare_index]<7){
                gshare_pred_table[gshare_index]++;
            }else if(predictor_used==1 && outcome=='n' && gshare_pred_table[gshare_index]>0){
                gshare_pred_table[gshare_index]--;
            }
        }
		
        
            bhr=bhr>>1;
            if(outcome=='t'){
                bhr=bhr+pow(2, n-1);    
            }    
		
	    strcpy(bin, "");
	    strcpy(bimodal_final_bin, "");
        strcpy(gshare_final_bin, "");
        strcpy(chooser_final_bin, "");
    }
    printf("COMMAND\n");
    if(type==0){
        printf("./sim %s %d %s\n", predictor, m_bimodal, argv[3]);
    }else if(type==1){
        printf("./sim %s %d %d %s\n", predictor, m_gshare, n, argv[4]);
    }else if(type==2){
        printf("./sim %s %d %d %d %d %s\n", predictor, k, m_gshare, n, m_bimodal, argv[6]);
    }
    printf("OUTPUT\n");
    printf("number of predictions: %d\n", predictions);
    printf("number of mispredictions: %d\n", misspredictions);
    printf("misprediction rate: %0.2f%%\n", ((float)misspredictions/predictions)*100);
    if(strncmp(predictor, "gshare", 6)==0){
        printf("FINAL GSHARE CONTENTS\n");
    }else if(strncmp(predictor, "bimodal", 7)==0){
        printf("FINAL BIMODAL CONTENTS\n");
    }

    if(type==0){
        for(i=0; i<bimodal_pred_table_size; i++){
            printf("%d\t%d\n", i, bimodal_pred_table[i]);
        }
    }else if(type==1){
        for(i=0; i<gshare_pred_table_size; i++){
            printf("%d\t%d\n", i, gshare_pred_table[i]);
        }
    }else if(type==2){
        printf("FINAL CHOOSER CONTENTS\n");
        for(i=0; i<chooser_table_size; i++){
            printf("%d\t%d\n", i, chooser_table[i]);
        }
        printf("FINAL GSHARE CONTENTS\n");
        for(i=0; i<gshare_pred_table_size; i++){
            printf("%d\t%d\n", i, gshare_pred_table[i]);
        }
        printf("FINAL BIMODAL CONTENTS\n");
        for(i=0; i<bimodal_pred_table_size; i++){
            printf("%d\t%d\n", i, bimodal_pred_table[i]);
        }
    }
}