#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG_FILE "/var/log/syslog"  // Syslog dosya yolu (Ubuntu/Debian i�in)

typedef struct LogNode {
    char data[1024];//sat�r�n max uzunlugunu temsil eder.
    struct LogNode* next;//bir sonraki d���me i�aret eder tek y�nl� liste olusturmam�z� sa�lar.
} LogNode;

// Yeni bir d���m olu�tur
LogNode* createNode(const char* log) {
    LogNode* newNode = (LogNode*)malloc(sizeof(LogNode));//bellekte yer ay�r�r
    if (newNode) {
        strncpy(newNode->data, log, 1023);//log'u newnode data'ya kopyalar
        newNode->data[1023] = '\0';  // G�venlik i�in son karakteri s�n�rlar
        newNode->next = NULL;//yeni d���m�n ba�ka d���m� i�aret etmmedi�ini g�sterir
    }
    return newNode;
}

// Listeye d���m ekler
void appendNode(LogNode** head, const char* log) { 
    LogNode* newNode = createNode(log);//yeni d���m ekler
    if (!newNode) return;

    if (*head == NULL) {//liste bo�sa yeni d���m ilk eleman olur
        *head = newNode;
    } else {//liste bo� de�ilse sona gidip newnode'u sonuncu d���m�n next i�aretcisine ba�lar�z
        LogNode* temp = *head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// Listede bulunan her d���m�n alan�n� yazd�r�r
void printList(LogNode* head) {
    LogNode* temp = head;
    while (temp) {
        printf("%s\n", temp->data);//d���m�n sat�r�n� ekrana basar
        temp = temp->next;//sonraki d���me ge�er
    }
}

// Listeyi temizler (haf�zay� bo�alt)
void freeList(LogNode* head) {
    LogNode* temp;
    while (head) {
        temp = head;//o anki d���m� i�aretciye al�r
        head = head->next;
        free(temp);//ge�ici i�aretciyle g�sterilen d���m� serbest b�rak�r
    }
}

int main() {
    FILE* file = fopen(LOG_FILE, "r");//dosyay� okuma modunda a�ar
    if (!file) {
        perror("Dosya a�ilamadi");
        return 1;
    }

    LogNode* logList = NULL;//ba�ta ba�l� liste bo� olsun diye null olarak ayarlan�r
    char line[1024];

    // Sat�r sat�r okur ve ba�l� listeye ekler
    while (fgets(line, sizeof(line), file)) {
        appendNode(&logList, line);
    }

    fclose(file);

    // Listeyi ekrana yazd�r�r
    printList(logList);

    // Haf�zay� temizler(ba�l� listedeki d���mleri bellekte serbest b�rak�r)
    freeList(logList);

    return 0;
}
