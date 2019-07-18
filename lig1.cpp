#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct lig lig;
typedef struct takim takim;//takma isim,uzun isim,oynadýðý mac sayisi,galibiyet sayisi,beraberlik sayisi,malubilyet sayisi,attigi gol sayisi,yedigi gol sayisi, averaj,puan 

struct lig{
	int **skor_matrisi;
	int **oynayanlar_matrisi;
	takim *takimlar;
	int takim_sayisi;
	int galibiyet_puani;
	int beraberlik_puani;
	int maglubiyet_puani;
};

struct takim{
	char isim[12];
	char kisa_isim;
	int oynanan_mac;
	int galibiyet_sayisi;
	int beraberlik_sayisi;
	int maglubiyet_sayisi;
	int atilan_gol;
	int yenilen_gol;
	int skor;
	int sira;
};

lig* lig_olustur();
takim* takim_olustur(char*,char);

void klavyeden_oku(lig*);
void dosyadan_takim_oku(lig*,char*);
void dosyadan_skor_oku(lig*,char*);
void ayarlari_dosyadan_oku(lig*,char*);
void takimlari_ligden_oku(lig*);
void skor_tablosunu_ligden_oku(lig*,int);
void sirala_puan(lig*);
void sirala_ad(lig*);
void sirala_takma_ad(lig*);
void swap(takim*,takim*);
char* buyut(char*);
char* kucult(char*);
void skoru_dosyaya_yazdir(lig*,const char*);

int main(int argc,char**argv){
	lig *l = lig_olustur();
	int secim = 1;
	while(1){
		printf("ayarlar icin 1'e, takimlari okumak icin 2'ye, skorlar icin 3'ye,tabloyu ekrana cizmek icin 4'e, dosyaya bastirmak icin 5'e basin\n");
		fflush(stdin);
		scanf("%d",&secim);
		switch(secim){
			case 1 : 
				ayarlari_dosyadan_oku(l,"ayarlar.txt");
				printf("ayarlar dosyadan okundu\n");
				break;
			case 2 :
				dosyadan_takim_oku(l,"takimlar.txt");
				printf("Takimlar dosyadan okundu\n");
				break;
			case 3 :
				printf("Skorlari klavyeden girmek icin 1'e, dosyadan girmek icin 2'ye basin\n");
				fflush(stdin);
				scanf("%d",&secim);
				switch(secim){
					case 1 :
						printf("Klavyeden degerleri giriniz:\n");
						while(secim){
							klavyeden_oku(l);
							printf("klavyeden input almak istiyorsaniz 1'e yoksa 0'a basin\n");
							fflush(stdin);
							scanf("%d",&secim);
						}break;
					case 2 : dosyadan_skor_oku(l,"skorlar.txt");break;
				}break;
			case 4 : 
				printf("puana gore siralamak icin 1,alfabetik siraya gore siralamak icin 2'ye,takma ada gore siralamak icin 3'e basin\n");
				fflush(stdin);
				scanf("%d",&secim);
				switch(secim){
					case 1 :
						sirala_puan(l);
						break;
					case 2 :
						sirala_ad(l);
						break;
					case 3 :
						sirala_takma_ad(l);
						break;
				}
				printf("Buyuk harf icin 1,kucuk harf icin 2'ye basin\n");
				scanf("%d",&secim);
				switch(secim){
					case 1 :
						skor_tablosunu_ligden_oku(l,1);break;
					case 2 :
						skor_tablosunu_ligden_oku(l,2);break;
				}break;
			case 5 :
				skoru_dosyaya_yazdir(l,"ekg.txt");
				break;
		}
		//takimlari_ligden_oku(l);
	}
}

void klavyeden_oku(lig *l){
	char birinci_takim, ikinci_takim;
	int birinci_skor,ikinci_skor;

	//klavyeden skorlari oku
	int secim = 0;
	fflush(stdin);
		if(scanf("%c %d %c %d",&birinci_takim,&birinci_skor,&ikinci_takim,&ikinci_skor) != 4){
				printf("secimler : %c %d %c %d\n",birinci_takim,birinci_skor,ikinci_takim,ikinci_skor);
				printf("input error\n");
			return;
		}
		if(l->oynayanlar_matrisi[birinci_takim - 65][ikinci_takim - 65]){
			printf("bu mac daha onceden oynandi\n");
			return;
		}
		printf("secimler : %c %d %c %d\n",birinci_takim,birinci_skor,ikinci_takim,ikinci_skor);
		int takim1_sira;
		int takim2_sira;
		//skorlarýn ait olduðu takým, matriste hangi indiste yer aliyor onu ogren	
		l->skor_matrisi[birinci_takim - 65][ikinci_takim - 65] = birinci_skor;
		l->skor_matrisi[ikinci_takim - 65][birinci_takim - 65] = ikinci_skor;
		l->takimlar[birinci_takim - 65].oynanan_mac++;
		l->takimlar[ikinci_takim - 65].oynanan_mac++;
		if(birinci_skor > ikinci_skor){
			l->takimlar[birinci_takim - 65].galibiyet_sayisi++;
			l->takimlar[ikinci_takim - 65].maglubiyet_sayisi++;
			l->takimlar[birinci_takim - 65].skor += l->galibiyet_puani;
			l->takimlar[ikinci_takim - 65].skor += l->maglubiyet_puani;
		}
		if(birinci_skor < ikinci_skor){
			l->takimlar[birinci_takim - 65].galibiyet_sayisi++;
			l->takimlar[ikinci_takim - 65].maglubiyet_sayisi++;
			l->takimlar[birinci_takim - 65].skor += l->galibiyet_puani;
			l->takimlar[ikinci_takim - 65].skor += l->maglubiyet_puani;
		}
		if(birinci_skor == ikinci_skor){
			l->takimlar[birinci_takim - 65].beraberlik_sayisi++;
			l->takimlar[ikinci_takim - 65].beraberlik_sayisi++;
			l->takimlar[birinci_takim - 65].skor += l->beraberlik_puani;
			l->takimlar[ikinci_takim - 65].skor += l->beraberlik_puani;
		}
	l->takimlar[birinci_takim - 65].atilan_gol += birinci_skor;
	l->takimlar[ikinci_takim - 65].atilan_gol += ikinci_skor;
	l->takimlar[birinci_takim - 65].yenilen_gol += ikinci_skor;
	l->takimlar[ikinci_takim - 65].yenilen_gol += birinci_skor;
}

lig* lig_olustur(){
	lig *l = (lig*)malloc(sizeof(lig));
	l->takimlar = NULL;
	l->takim_sayisi = 0;
	l->galibiyet_puani = 2;
	l->beraberlik_puani = 1;
	l->maglubiyet_puani = -1;
	l->skor_matrisi = NULL;
	l->oynayanlar_matrisi = NULL;
	return l;
}

takim *takim_olustur(char *isim,char kisa_isim){
	takim *t = (takim*)malloc(sizeof(takim));
	strcpy(t->isim,isim);
	t->oynanan_mac = 0;
	t->galibiyet_sayisi = 0;
	t->beraberlik_sayisi = 0;
	t->maglubiyet_sayisi = 0;
	t->atilan_gol = 0;
	t->yenilen_gol = 0;
	t->skor = 0;
	t->kisa_isim = kisa_isim;
	return t;
}

void ayarlari_dosyadan_oku(lig *l, char *dosya_adi){
	FILE *fp = fopen(dosya_adi,"r");
	int takim_sayisi,galibiyet_puani,maglubiyet_puani,beraberlik_puani;
	fscanf(fp,"%d %d %d %d\0",&takim_sayisi,&galibiyet_puani,&maglubiyet_puani,&beraberlik_puani);
	l->takim_sayisi = takim_sayisi;
	l->galibiyet_puani = galibiyet_puani;
	l->maglubiyet_puani = maglubiyet_puani;
	l->beraberlik_puani = beraberlik_puani;

	fclose(fp);
}

void takimlari_ligden_oku(lig *l){
	printf("--- Takim listesi ---\n\n");
	for(int i = 0;i < l->takim_sayisi;i++){
		printf("%c : %s\n",l->takimlar[i].kisa_isim,l->takimlar[i].isim);
	}
	printf("\n\n");
}

void skor_tablosunu_ligden_oku(lig *l,int secim){
	char kisa_isim1,kisa_isim2;
	char *isim1,*isim2;
	
	printf("\tT.I\t\tU.I\tO.M\tG\tB\tM\tA.G\tY.G\tA\tP\n\n");
	for(int i = 0;i < l->takim_sayisi;i++){
		char kisa_isim = l->takimlar[i].kisa_isim;
		char *isim = (secim == 1)? buyut(l->takimlar[i].isim) : kucult(l->takimlar[i].isim);
		int oynanan_mac = l->takimlar[i].oynanan_mac;
		int galibiyet = l->takimlar[i].galibiyet_sayisi;
		int beraberlik = l->takimlar[i].beraberlik_sayisi;
		int maglubiyet = l->takimlar[i].maglubiyet_sayisi;
		int atilan_gol = l->takimlar[i].atilan_gol;
		int yedigi_gol = l->takimlar[i].yenilen_gol;
		int averaj = atilan_gol - yedigi_gol;
		int puan = l->takimlar[i].skor;
		printf("\t%c\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",kisa_isim,isim,oynanan_mac,galibiyet,beraberlik,maglubiyet,atilan_gol,yedigi_gol,averaj,puan);
	}
	printf("\n\n");
}

void dosyadan_takim_oku(lig *l,char *dosya_adi){
	FILE *fp = fopen(dosya_adi,"r");
	char takim_adi[12];
	while(!feof(fp)){
		fscanf(fp,"%s",takim_adi);
		//eger dosya sonuna geldiysek donguden cik.
		if(feof(fp))break;
		//yeni takim olusturuldu
		takim *yeni_takim = takim_olustur(takim_adi,l->takim_sayisi + 65);
		yeni_takim->sira = l->takim_sayisi++;
		//skor matrisinin boyutlari arttirildi
		l->skor_matrisi = (int**)realloc(l->skor_matrisi,sizeof(int*) * l->takim_sayisi);
		l->skor_matrisi[l->takim_sayisi - 1] = (int*)calloc(l->takim_sayisi,sizeof(int));
		
		l->oynayanlar_matrisi = (int**)realloc(l->oynayanlar_matrisi,sizeof(int*) * l->takim_sayisi);
		l->oynayanlar_matrisi[l->takim_sayisi - 1] = (int*)calloc(l->takim_sayisi,sizeof(int));
		
		for(int i = 0;i < l->takim_sayisi;i++){
			l->skor_matrisi[i] = (int*)realloc(l->skor_matrisi[i],sizeof(int) * l->takim_sayisi);
			l->skor_matrisi[i][l->takim_sayisi - 1] = 0;
			
			l->oynayanlar_matrisi[i] = (int*)realloc(l->oynayanlar_matrisi[i],sizeof(int) * l->takim_sayisi);
			l->oynayanlar_matrisi[i][l->takim_sayisi - 1] = 0;
		}

		//takimlar dizisinin boyutu arttýrýldý
		l->takimlar = (takim*)realloc(l->takimlar,sizeof(takim) * l->takim_sayisi + 1);
		//yeni takim eklendi
		l->takimlar[l->takim_sayisi - 1] = *yeni_takim;
	}
	fclose(fp);
}

void dosyadan_skor_oku(lig *l,char *dosya_adi){
	FILE *fp = fopen(dosya_adi,"r");
	char birinci_takim, ikinci_takim;
	int birinci_skor,ikinci_skor;

	//dosyadan skorlari oku
	while(fscanf(fp,"%c %d %c %d\n",&birinci_takim,&birinci_skor,&ikinci_takim,&ikinci_skor)){
		if(l->oynayanlar_matrisi[birinci_takim - 65][ikinci_takim - 65]){
			printf("Daha onceden oynandi\n");
			break;
		}
		l->skor_matrisi[birinci_takim - 65][ikinci_takim - 65] = birinci_skor;
		l->skor_matrisi[ikinci_takim - 65][birinci_takim - 65] = ikinci_skor;
		
		l->oynayanlar_matrisi[birinci_takim - 65][ikinci_takim - 65] = 1;
		
		l->takimlar[birinci_takim - 65].oynanan_mac++;
		l->takimlar[ikinci_takim - 65].oynanan_mac++;
		if(birinci_skor > ikinci_skor){
			l->takimlar[birinci_takim - 65].galibiyet_sayisi++;
			l->takimlar[ikinci_takim - 65].maglubiyet_sayisi++;
			l->takimlar[birinci_takim - 65].skor += l->galibiyet_puani;
			l->takimlar[ikinci_takim - 65].skor += l->maglubiyet_puani;
		}
		if(birinci_skor < ikinci_skor){
			l->takimlar[ikinci_takim - 65].galibiyet_sayisi++;
			l->takimlar[birinci_takim - 65].maglubiyet_sayisi++;
			l->takimlar[ikinci_takim - 65].skor += l->galibiyet_puani;
			l->takimlar[birinci_takim - 65].skor += l->maglubiyet_puani;
		}
		if(birinci_skor == ikinci_skor){
			l->takimlar[birinci_takim - 65].beraberlik_sayisi++;
			l->takimlar[ikinci_takim - 65].beraberlik_sayisi++;
			l->takimlar[birinci_takim - 65].skor += l->beraberlik_puani;
			l->takimlar[ikinci_takim - 65].skor += l->beraberlik_puani;
		}
		l->takimlar[birinci_takim - 65].atilan_gol += birinci_skor;
		l->takimlar[ikinci_takim - 65].atilan_gol += ikinci_skor;
		l->takimlar[birinci_takim - 65].yenilen_gol += ikinci_skor;
		l->takimlar[ikinci_takim - 65].yenilen_gol += birinci_skor;
		if(feof(fp))break;
	}
	fclose(fp);
}

void sirala_puan(lig *l){
	for(int i = 0;i < l->takim_sayisi - 1;i++){
		for(int j = 0;j < l->takim_sayisi - i - 1;j++){
			if(l->takimlar[j].skor < l->takimlar[j + 1].skor){
				swap(&l->takimlar[j],&l->takimlar[j + 1]);	
			}
		}
	}
}

void sirala_ad(lig *l){
	for(int i = 0;i < l->takim_sayisi - 1;i++){
		for(int j = 0;j < l->takim_sayisi - i - 1;j++){
			if(strcmp(l->takimlar[j].isim,l->takimlar[j + 1].isim) > 0){
				swap(&l->takimlar[j],&l->takimlar[j + 1]);	
			}
		}
	}
}

void sirala_takma_ad(lig *l){
	for(int i = 0;i < l->takim_sayisi - 1;i++){
		for(int j = 0;j < l->takim_sayisi - i - 1;j++){
			if(l->takimlar[j].kisa_isim > l->takimlar[j + 1].kisa_isim){
				swap(&l->takimlar[j],&l->takimlar[j + 1]);	
			}
		}
	}
}

void swap(takim *t1,takim *t2){
	char isim[12];
	char kisa_isim;
	int oynanan_mac;
	int galibiyet_sayisi;
	int beraberlik_sayisi;
	int maglubiyet_sayisi;
	int atilan_gol;
	int yenilen_gol;
	int skor;
	int sira;
	strcpy(isim,t1->isim);
	kisa_isim = t1->kisa_isim;
	oynanan_mac = t1->oynanan_mac;
	galibiyet_sayisi = t1->galibiyet_sayisi;
	beraberlik_sayisi = t1->beraberlik_sayisi;
	maglubiyet_sayisi = t1->maglubiyet_sayisi;
	atilan_gol = t1->atilan_gol;
	yenilen_gol = t1->yenilen_gol;
	skor = t1->skor;
	sira = t1->sira;
	strcpy(t1->isim,t2->isim);
	t1->kisa_isim = t2->kisa_isim;
	t1->oynanan_mac = t2->oynanan_mac;
	t1->galibiyet_sayisi = t2->galibiyet_sayisi;
	t1->beraberlik_sayisi = t2->beraberlik_sayisi;
	t1->maglubiyet_sayisi = t2->maglubiyet_sayisi;
	t1->atilan_gol = t2->atilan_gol;
	t1->yenilen_gol = t2->yenilen_gol;
	t1->skor = t2->skor;
	t1->sira = t2->sira;
	
	strcpy(t2->isim,isim);
	t2->kisa_isim = kisa_isim;
	t2->oynanan_mac = oynanan_mac;
	t2->galibiyet_sayisi = galibiyet_sayisi;
	t2->beraberlik_sayisi = beraberlik_sayisi;
	t2->maglubiyet_sayisi = maglubiyet_sayisi;
	t2->atilan_gol = atilan_gol;
	t2->yenilen_gol = yenilen_gol;
	t2->skor = skor;
	t2->sira = sira;
}

//takimin lige kaçýncý sýradan eklendigini takim kisaltmasindan bul
int kisaltmadan_indis_don(lig *l,char kisaltma){
	int takim_sirasi;
	for(int i = 0;i < l->takim_sayisi;i++){
		if(kisaltma == l->takimlar[i].kisa_isim){
			takim_sirasi = i;
			break;
		}
	}
	return takim_sirasi;
}

char* buyut(char *str){
	char *str2 = (char*)malloc(sizeof(char) * (strlen(str)+1));
	strcpy(str2,str);
	char *ptr = str2;
	while(*ptr){
		if(*ptr > 90){
			*ptr -= 32;
		}
		ptr++;
	}
	return str2;
}

char* kucult(char *str){
	char *str2 = (char*)malloc(sizeof(char) * (strlen(str)+1));
	strcpy(str2,str);
	char *ptr = str2;
	while(*ptr){
		if(*ptr < 97){
			*ptr += 32;
		}
		ptr++;
	}
	return str2;
}

void skoru_dosyaya_yazdir(lig *l,const char *dosya_adi){
	char kisa_isim1,kisa_isim2;
	char *isim1,*isim2;
	
	FILE *fp = fopen(dosya_adi,"w");
	if(!fp){
		printf("dosya acilamadi\n");
	}
	fprintf(fp,"\tT.I\t\tU.I\tO.M\tG\tB\tM\tA.G\tY.G\tA\tP\n\n");
	for(int i = 0;i < l->takim_sayisi;i++){
		char kisa_isim = l->takimlar[i].kisa_isim;
		char *isim = l->takimlar[i].isim;
		int oynanan_mac = l->takimlar[i].oynanan_mac;
		int galibiyet = l->takimlar[i].galibiyet_sayisi;
		int beraberlik = l->takimlar[i].beraberlik_sayisi;
		int maglubiyet = l->takimlar[i].maglubiyet_sayisi;
		int atilan_gol = l->takimlar[i].atilan_gol;
		int yedigi_gol = l->takimlar[i].yenilen_gol;
		int averaj = atilan_gol - yedigi_gol;
		int puan = l->takimlar[i].skor;
		fprintf(fp,"\t%c\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",kisa_isim,isim,oynanan_mac,galibiyet,beraberlik,maglubiyet,atilan_gol,yedigi_gol,averaj,puan);
	}
	fprintf(fp,"\n\n");
	fclose(fp);
}


