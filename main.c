#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG_FILE "/var/log/syslog"  // Syslog dosya yolu (Ubuntu/Debian için)

typedef struct LogNode {
    char data[1024];//satýrýn max uzunlugunu temsil eder.
    struct LogNode* next;//bir sonraki düðüme iþaret eder tek yönlü liste olusturmamýzý saðlar.
} LogNode;

// Yeni bir düðüm oluþtur
LogNode* createNode(const char* log) {
    LogNode* newNode = (LogNode*)malloc(sizeof(LogNode));//bellekte yer ayýrýr
    if (newNode) {
        strncpy(newNode->data, log, 1023);//log'u newnode data'ya kopyalar
        newNode->data[1023] = '\0';  // Güvenlik için son karakteri sýnýrlar
        newNode->next = NULL;//yeni düðümün baþka düðümü iþaret etmmediðini gösterir
    }
    return newNode;
}

// Listeye düðüm ekler
void appendNode(LogNode** head, const char* log) { 
    LogNode* newNode = createNode(log);//yeni düðüm ekler
    if (!newNode) return;

    if (*head == NULL) {//liste boþsa yeni düðüm ilk eleman olur
        *head = newNode;
    } else {//liste boþ deðilse sona gidip newnode'u sonuncu düðümün next iþaretcisine baðlarýz
        LogNode* temp = *head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// Listede bulunan her düðümün alanýný yazdýrýr
void printList(LogNode* head) {
    LogNode* temp = head;
    while (temp) {
        printf("%s\n", temp->data);//düðümün satýrýný ekrana basar
        temp = temp->next;//sonraki düðüme geçer
    }
}

// Listeyi temizler (hafýzayý boþalt)
void freeList(LogNode* head) {
    LogNode* temp;
    while (head) {
        temp = head;//o anki düðümü iþaretciye alýr
        head = head->next;
        free(temp);//geçici iþaretciyle gösterilen düðümü serbest býrakýr
    }
}

int main() {
    FILE* file = fopen(LOG_FILE, "r");//dosyayý okuma modunda açar
    if (!file) {
        perror("Dosya açilamadi");
        return 1;
    }

    LogNode* logList = NULL;//baþta baðlý liste boþ olsun diye null olarak ayarlanýr
    char line[1024];

    // Satýr satýr okur ve baðlý listeye ekler
    while (fgets(line, sizeof(line), file)) {
        appendNode(&logList, line);
    }

    fclose(file);

    // Listeyi ekrana yazdýrýr
    printList(logList);

    // Hafýzayý temizler(baðlý listedeki düðümleri bellekte serbest býrakýr)
    freeList(logList);

    return 0;
}
