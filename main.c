#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define USERNAME 92668751
#define PASSWORD 2476627037

typedef struct { // hastaya ait veri yapýsý
	int id;
	char name[25];
	int health_score;
} Patient;

typedef struct { // hastalarýn tutulmasý için kullanýlan veri yapýsý
	Patient *data[200];
	int size;
	int cap;
} PatientStore;

void add_patient(PatientStore *store);
void delete_by_id(PatientStore *store);
void list_patients(PatientStore *store);
void update_scores_by_id(PatientStore *store);
void search_by_name(PatientStore *store);
void sort_by_average(PatientStore *store);
int save_patients_to_csv(const PatientStore *store);
int load_patients_from_csv(PatientStore *store);
unsigned int hash(const char *str);

int main() {
	
	write_log("Program baslatildi.");

	PatientStore store;
	int choice;
	
	store.size = 0; // baþlangýçta mevcut hasta sayýsý 0
	store.cap = 200; // hastane kapasitesi (200 örnektir)
	
	
	if(login()==1) {
		do {
		printf("\n>>> HASTANE VERI YONETIMI SISTEMI <<<\n");
		printf("-------------------------------------\n");
		printf("1. Hasta Ekle\n");
		printf("2. Hasta Sil\n");
		printf("3. Hastalari Listele\n");
		printf("4. Saglik Puani Guncelle\n");
		printf("5. Isme Gore Hasta Ara\n");
		printf("6. Hastalari Saglik Puanlarina Gore Sirala\n");
		printf("7. Sistemdeki Hastalari CSV'ye Yazdir\n");
		printf("8. CSV'deki Hastalari Sisteme Yukle\n");
		printf("9. Sistemdeki Hastalari JSON'a Yazdir\n");
		printf("10. JSON'daki Hastalari Sisteme Yukle\n");
		printf("0. Cikis\n");
		printf("-------------------------------------");
		printf("\nSeciminiz: ");
		scanf("%d", &choice);
	
			switch(choice) {
				case 1:
					add_patient(&store);
					break;
				case 2:
					delete_by_id(&store);
					break;
				case 3:
					list_patients(&store);
					break;
				case 4:
					update_scores_by_id(&store);
					break;
				case 5:
					search_by_name(&store);
					break;
				case 6:
					sort_by_average(&store);
					break;
				case 7:
					if(save_patients_to_csv(&store)==1) {
						printf("-------------------------------------\n");
						printf("Sisteme kayitli tum hastalar CSV dosyasina yazdirildi!\n\n\n");
						write_log("CSV kaydetme basarili.");
						break;
					}
					else {
						printf("-------------------------------------\n");
						printf("Sistemde kayitli hasta bulunmadiginden CSV dosyasina yazdirma basarisiz oldu.\n\n\n");
						write_log("CSV kaydetme basarisiz.");
						break;
					}
				case 8:
					if(load_patients_from_csv(&store)==1) {
						printf("-------------------------------------\n");
						printf("CSV dosyasindaki hastalar basariyla sisteme yuklendi!\n\n\n");
						write_log("CSV'den yazdirma basarili.");
						break;
				 	}
					else {
						printf("-------------------------------------\n");
						printf("CSV dosyasinda yuklenebilir hasta bulunamadi.\n\n\n");
						write_log("CSV'den yazdirma basarisiz.");
						break;
					}
				case 9:
					if(save_patients_to_json(&store)==1) {
						printf("-------------------------------------\n");
						printf("Sisteme kayitli hastalar JSON dosyasina yazdirildi!\n\n\n");
						write_log("JSON kaydetme basarili.");
						break;
					}
					else {
						printf("-------------------------------------\n");
						printf("Sistemde kayitli hasta bulunmadigindan JSON dosyasina yazdirma basarisiz oldu.\n\n\n");
						write_log("JSON kaydetme basarisiz.");
						break;
					}
				case 10:
					if(load_patients_from_json(&store)==1) {
						printf("-------------------------------------\n");
						printf("JSON dosyasindaki tum hastalar basariyla sisteme yuklendi!\n\n\n");
						write_log("JSON'dan yazdirma basarili.");
						break;
					}
					else {
						printf("-------------------------------------\n");
						printf("JSON dosyasinda yuklenebilir hasta bulunamadi.\n\n\n");
						write_log("JSON'dan yazdirma basarisiz.");
						break;
					}
				case 0:
					printf("-------------------------------------\n");
					printf("Programdan cikis yapiliyor...\n");
					write_log("Program kapatildi.");
					break;
				default:
					printf("Gecersiz bir secim yaptiniz.\n");
			}
		} while(choice!=0);
	} 
	else {
		return 0;
	}
}

void add_patient(PatientStore *store) {
	if(store->size >= store->cap) { // mevcut hasta sayýsý, hastane kapasitesine eþitse hata verir
		printf("Hastane kapasitesi dolu!\n");
		return;
	}
	
	Patient *p = (Patient *)malloc(sizeof(Patient));
	if(p == NULL) { // bellekte boþ yer yoksa hata verir
		printf("Bellek ayirma basarisiz!\n");
		return;
	}
	
	printf("-------------------------------------");
	printf("\nHasta ID'sini girin: ");
	scanf("%d", &p->id);
	
	printf("Hasta adini girin: ");
	scanf("%s", p->name);
	
	printf("Hasta saglik puanini girin: ");
	scanf("%d", &p->health_score);
	printf("-------------------------------------");
	
	printf("\nHasta basariyla eklendi!\n\n\n");
	write_log("Hasta eklendi.");
	
	store->data[store->size] = p; // eklenen hastayý diziye kaydeder
	store->size++; // mevcut hasta sayýsýný 1 arttýrýr

}

void delete_by_id(PatientStore *store) {
	int tempID;
	int i, j;
	printf("-------------------------------------\n");
	printf("Silmek istediginiz hasta ID'sini girin: ");
	scanf("%d", &tempID);
	printf("-------------------------------------\n");
	for(i = 0; i < store->size; i++) {
		if (store->data[i]->id == tempID) { // tüm hastalarýn ID'lerini dolaþýr ve girilen ile eþit mi kontrol eder
			
			free(store->data[i]); // o hastaya ait verileri diziden kaldýrýr
			
			for(j = i; j < store->size - 1; j++) {
				store->data[j] = store->data[j+1]; // dizi sýrasýnýn karýþmamasý için her elemaný bir sola kaydýrýr
			}
			
			store->size--; // mevcut hasta sayýsýný 1 azaltýr
			
			printf("Hasta basariyla silindi!\n\n\n");
			write_log("Hasta silindi.");
			save_patients_to_csv(store);
			save_patients_to_json(store);
			return;
		}
	}
	
	printf("Aradiginiz hasta bulunamadi.\n\n\n");
}

void list_patients(PatientStore *store) {
    int i;

    if (store->size == 0) {
    	printf("-------------------------------------\n");
        printf("Kayitli hasta bulunamadi.\n\n\n");
    }
    else {
    	printf("\nTum kayitli hastalar listeleniyor:");
        printf("\n---------------------------------\n");
        printf("| %-10s | %-3s | %-12s |\n", "Hasta", "ID", "Saglik Puani"); // tablo formatý
        printf("---------------------------------\n");

        for (i = 0; i < store->size; i++) {
            printf("| %-10s | %-3d | %-12d |\n",
                   store->data[i]->name,
                   store->data[i]->id,
                   store->data[i]->health_score);
        }

        printf("---------------------------------\n\n\n");
    }
}


void update_scores_by_id(PatientStore *store) {
	int tempID, tempScore;
	int i;
	printf("-------------------------------------\n");
	printf("Saglik puanini guncellemek istediginiz hastanin ID'sini girin: ");
	scanf("%d", &tempID);
	
	for(i = 0; i < store->size; i++) {
		if(store->data[i]->id == tempID) { // tüm hastalarýn ID'lerini dolaþýr ve girilen ile eþit mi kontrol eder
			printf("Hastanin yeni saglik puanini girin: ");
			scanf("%d", &tempScore);
			store->data[i]->health_score = tempScore; // eski saðlýk puanýný girilen ile deðiþtirir
			printf("-------------------------------------\n");
			printf("%s adli hastanin saglik puani %d olarak guncellendi!\n\n\n", store->data[i]->name, store->data[i]->health_score);
			return;
		}
	}
	printf("Aradiginiz hasta bulunamadi.\n\n\n");
}

void search_by_name(PatientStore *store) {
    char tempName[20];
    int i;
	printf("-------------------------------------\n");
    printf("Aradiginiz hastanin adini girin: ");
    scanf("%19s", tempName);
    printf("-------------------------------------\n");

    for (i = 0; i < store->size; i++) {
        if (strcmp(store->data[i]->name, tempName) == 0) { // tüm hastalarýn isimlerini dolaþýr, girilen ile ayný mý kontrol eder
            printf("Hasta bulundu!\n");
            printf("ID: %d\n", store->data[i]->id);
            printf("Isim: %s\n", store->data[i]->name);
            printf("Saglik Puani: %d\n", store->data[i]->health_score);
            printf("-------------------------------------\n\n\n");
            return;
        }
    }
    printf("Aradiginiz hasta bulunamadi.\n\n\n");
}

void sort_by_average(PatientStore *store) {
    int i, j;
    Patient *temp;

    if (store->size < 2) {
    	printf("-------------------------------------\n");
        printf("Siralama yapilmasi icin en az kayitli 2 hasta olmasi gerekmektedir.\n\n\n");
        return;
    }

    for (i = 0; i < store->size - 1; i++) {
        for (j = 0; j < store->size - i - 1; j++) {
        	// tüm hastalarýn puanlarýný dolaþýr, küçük ile büyük puanlarýn yerlerini deðiþtirip büyükten küçüðe sýralama yapar
            if (store->data[j]->health_score < store->data[j + 1]->health_score) {
                temp = store->data[j];
                store->data[j] = store->data[j + 1];
                store->data[j + 1] = temp;
            }
        }
    }
	printf("-------------------------------------\n");
    printf("\nHastalar saglik puanina gore siralandi.\n");
    list_patients(store); // güncel sýra ile listeleme fonksiyonu çaðrýlýr
}

int save_patients_to_csv(const PatientStore *store) {
	int i;
	FILE *dosya;
	dosya = fopen("hastalar.csv", "w");
	if(dosya==NULL) {
		printf("Dosya bulunamadi!\n");
		return 0;
	}
	
	if(store->size == 0) {
		return 0;
	}
	
	fprintf(dosya, "ID;Isim;SaglikPuani\n");
	
	for(i=0; i<store->size; i++) {
		fprintf(dosya, "%d;%s;%d\n", store->data[i]->id, store->data[i]->name, store->data[i]->health_score);
	}
	fclose(dosya);
	return 1;
}

int load_patients_from_csv(PatientStore *store) {
	FILE *dosya;
	dosya = fopen("hastalar.csv", "r");
	if(dosya==NULL) {
		printf("CSV dosyasi bulunamadi!\n");
		return 0;
	}
	int yuklenen = 0;
	
	fscanf(dosya, "%*[^\n]\n");
	
	while(1) {
		if(store->size >= store->cap) {
			break;
		}
		
		Patient *p = malloc(sizeof(Patient));
		if(p==NULL) {
			break;
		}

		int okunan = fscanf(dosya, "%d;%[^;];%d\n",
							&p->id,
							p->name,
							&p->health_score);
		
		if(okunan == 3) {
			store->data[store->size] = p;
			store->size++;
			yuklenen++;
		}
		else {
			free(p);
			break;
		}
	}
	
	fclose(dosya);
	return yuklenen > 0;
}

int save_patients_to_json(const PatientStore *store) {

    // HÝÇ HASTA YOKSA
    if (store->size == 0) {
        printf("JSON dosyasina kaydedilecek hasta bulunamadi!\n");
        return 0;
    }

    FILE *dosya = fopen("hastalar.json", "w");
    if (dosya == NULL) {
        printf("JSON dosyasi olusturulamadi!\n");
        return 0;
    }

    fprintf(dosya, "{\n  \"patients\": [\n");
	int i;
    for (i = 0; i < store->size; i++) {
        fprintf(dosya,
                "    { \"id\": %d, \"name\": \"%s\", \"health_score\": %d }",
                store->data[i]->id,
                store->data[i]->name,
                store->data[i]->health_score);

        if (i != store->size - 1)
            fprintf(dosya, ",");

        fprintf(dosya, "\n");
    }

    fprintf(dosya, "  ]\n}\n");

    fclose(dosya);
    return 1;
}



int load_patients_from_json(PatientStore *store) {
    FILE *dosya = fopen("hastalar.json", "r");
    if (dosya == NULL) {
        printf("JSON dosyasi bulunamadi!\n");
        return 0;
    }

    char line[256];

    while (fgets(line, sizeof(line), dosya)) {
        if (strstr(line, "\"id\"")) {

            if (store->size >= store->cap)
                break;

            Patient *p = malloc(sizeof(Patient));
            if (p == NULL)
                break;

            int okunan = sscanf(line,
                " %*[^0-9]%d%*[^'\"]\"%[^\"]\"%*[^0-9]%d",
                &p->id,
                p->name,
                &p->health_score);

            if (okunan == 3) {
                store->data[store->size++] = p;
                return 1;
            } else {
                free(p);
            }
        }
    }

    fclose(dosya);
}

void write_log(const char *message) {
    FILE *log = fopen("log.txt", "a");
    if (log == NULL) return;

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(log, "[%s] %s\n", time_str, message);
    fclose(log);
}

int login() {
	int n = 3;
	char name[12];
	char pass[12];
	printf("======================================\n");
	printf("> Kullanici adinizi girin: ");
	gets(name);
	printf("======================================\n");
	printf("> Sifrenizi girin: ");
	gets(pass);
	printf("======================================\n");
	
	if(hash(name)==USERNAME && hash(pass)==PASSWORD) {
		write_log("Giris yapildi.");
		printf("> Giris basarili! Hos geldiniz, %s.\n", name);
		printf("======================================\n");
		return 1;
	}
	else {
		while(hash(name)!=USERNAME || hash(pass)!=PASSWORD) {
			n--;
			if(n==0) {
				write_log("Giris yapilamadigindan program kapatildi.");
				printf("> Tum giris denemeleriniz basarisiz oldugundan girisiniz engellenmistir.\n");
				return 0;
			}
			printf("> Hatali giris kombinasyonu! Kalan deneme hakkiniz: %d\n", n);
			printf("======================================\n");
			printf("> Kullanici adinizi girin: ");
			gets(name);
			printf("======================================\n");
			printf("> Sifrenizi girin: ");
			gets(pass);
			printf("======================================\n");
		}
		if(n!=0) {
			write_log("Giris yapildi.");
			printf("> Giris basarili! Hos geldiniz, %s.\n", name);
			printf("======================================\n");
			return 1;
		}
	}
}

unsigned int hash(const char *str) {
	unsigned int hash = 0;
	
	while(*str) {
		hash = hash * 31 + (unsigned char)(*str);
		str++;
	}
	
	return hash;
}
