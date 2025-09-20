#include <stdio.h>		//library yang digunakan
#include <string.h>		//library yang digunakan
#include <stdlib.h>		//library yang digunakan
#include <time.h>		//library yang digunakan
#include <stdbool.h>	//library yang digunakan

struct Node {			//Declare struct yang berisi inputan dan height yang digunakan program
    char name[51];
    char classes[21];
    char jurusan[21];
    int nisn;
    char born[41];
    int grad;

    int height;
    struct Node* left;	//karena ini avl tree jadi ada kanan kiri dan height
    struct Node* right;
}*root = NULL;

bool deleteFound = false; // Untuk cek ada yang bisa di delete atau tidak

struct Node* createNode(char name[], char classes[], char jurusan[], int nisn, char born[], int grad) {
    struct Node* newNode = (struct Node*) malloc (sizeof(struct Node));
    strcpy(newNode->name, name);
    strcpy(newNode->classes, classes);
    strcpy(newNode->jurusan, jurusan);	//fuction ini untuk masukin data ke program
    strcpy(newNode->born, born);
    newNode->nisn = nisn;
    newNode->grad = grad;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;

    return newNode;
}

int max(int a, int b) {		//untuk cari tau max nya dimana
    return (a > b) ? a : b;
}

int getHeight(struct Node* node) {	//untuk mendapatkan nilai tertinggi
    if(node == NULL) return 0;
    return node->height;
}

int setHeight(struct Node* node) { //untuk menjadikan node tersebut adalah nilai tertinggi
    return max(getHeight(node->left), getHeight(node->right)) + 1;
}

int getBalanceFactor(struct Node* node) { //karna ini avl jadi ada balancing
    return getHeight(node->left) - getHeight(node->right);
}

struct Node* rotateLeft(struct Node* x) { //untuk balancing
    struct Node* y = x->right;
    struct Node* z = y->left;

    x->right = z;
    y->left = x;

    x->height = setHeight(x);
    y->height = setHeight(y);

    return y;
}

struct Node* rotateRight(struct Node* x) { //untuk balancing
    struct Node* y = x->left;
    struct Node* z = y->right;

    x->left = z;
    y->right = x;

    x->height = setHeight(x);
    y->height = setHeight(y);

    return y;
}

struct Node* insertNode(struct Node* curr, char name[], char classes[], char jurusan[], char born[], int nisn, int grad) {
	// fuction ini untuk insert dan langsung di set sebagai avl tree
    if (curr == NULL) return createNode(name, classes, jurusan, nisn, born, grad);

    if (nisn < curr->nisn) {
        curr->left = insertNode(curr->left, name, classes, jurusan, born, nisn, grad);
    } else if (nisn > curr->nisn) {
        curr->right = insertNode(curr->right, name, classes, jurusan, born, nisn, grad);
    } else {
        return curr;
    }

    curr->height = setHeight(curr);
    int bf = getBalanceFactor(curr);

    if (bf > 1) {
        if (nisn < curr->left->nisn) return rotateRight(curr);
        else {
            curr->left = rotateLeft(curr->left);
            return rotateRight(curr);
        }
    } else if (bf < -1) {
        if (nisn > curr->right->nisn) return rotateLeft(curr);
        else {
            curr->right = rotateRight(curr->right);
            return rotateLeft(curr);
        }
    }
    return curr;
}

struct Node* deleteNode(struct Node* curr, int nisn) {
	// function ini untuk delete dan langsung balancing sebagai avl
    if(curr == NULL) return NULL;

    if(nisn < curr->nisn) {
        curr->left = deleteNode(curr->left, nisn);
    } else if(nisn > curr->nisn) {
        curr->right = deleteNode(curr->right, nisn);
    } else {
        deleteFound = true;
        if(curr->left == NULL && curr->right == NULL) {
            free(curr);
            return NULL;
        }
        if(curr->left == NULL) {
            struct Node* temp = curr->right;
            free(curr);
            return temp;
        } else if(curr->right == NULL) {
            struct Node* temp = curr->left;
            free(curr);
            return temp;
        }

        struct Node* is = curr->right;
        while(is->left != NULL) {
            is = is->left;
        }

        strcpy(curr->name, is->name);
        strcpy(curr->classes, is->classes);
        strcpy(curr->jurusan, is->jurusan);
        curr->nisn = is->nisn;
        strcpy(curr->born, is->born);
        curr->grad = is->grad;

        curr->right = deleteNode(curr->right, is->nisn);
    }

    curr->height = setHeight(curr);

    int bf = getBalanceFactor(curr);

    if(bf > 1) {
        if(getBalanceFactor(curr->left) >= 0) curr = rotateRight(curr);
        else {
            curr->left = rotateLeft(curr->left);
            curr = rotateRight(curr);
        }
    } else if (bf < -1) {
        if(getBalanceFactor(curr->right) <= 0) curr = rotateLeft(curr);
        else {
            curr->right = rotateRight(curr->right);
            curr = rotateLeft(curr);
        }
    }

    return curr;
}

void printRow(int nisn, char name[], char born[], char classes[], char jurusan[], int grad) {
	// function ini untuk print satuan data
    printf("| %d | %-25s | %-10s | %-5s | %-5s | %-15d |\n", nisn, name, born, classes, jurusan, grad);
    printf("-----------------------------------------------------------------------------------------\n");
}

void viewMajor(struct Node* curr, char jurusan[], int *countMajor) {
	//untuk cek view jurusan dan hitung ada berapa jurusan ipa/ips
    if (curr == NULL) return;

    viewMajor(curr->left, jurusan, countMajor);

    if (strcmp(curr->jurusan, jurusan) == 0) {
    	printf("-----------------------------------------------------------------------------------------\n");
		printf("| NISN 	     | Name 			 | Birth Date | Class | Major | Graduation Year |\n");
		printf("-----------------------------------------------------------------------------------------\n");
        printRow(curr->nisn, curr->name, curr->born, curr->classes, curr->jurusan, curr->grad);
        (*countMajor)++;
    }

    viewMajor(curr->right, jurusan, countMajor);
}

void searchNISN(struct Node* curr, int nisn, int *countNISN) {
	// untuk view nisn serta cari nisn yang di input
    if (curr == NULL) return;

    searchNISN(curr->left, nisn, countNISN);

    if (curr->nisn == nisn) {
    	printf("-----------------------------------------------------------------------------------------\n");
		printf("| NISN 	     | Name 			 | Birth Date | Class | Major | Graduation Year |\n");
		printf("-----------------------------------------------------------------------------------------\n");
        printRow(curr->nisn, curr->name, curr->born, curr->classes, curr->jurusan, curr->grad);
        (*countNISN)++;
    }

    searchNISN(curr->right, nisn, countNISN);
}

void viewData(struct Node* curr, int choose, bool *headerPrinted) {
	//untuk view menu
    if (root == NULL) {
        printf("No data available!\n");
        return;
    }

	if (!(*headerPrinted) && (choose == 1 || choose == 2 || choose == 3)) {
        printf("-----------------------------------------------------------------------------------------\n");
		printf("| NISN 	     | Name 			 | Birth Date | Class | Major | Graduation Year |\n");
		printf("-----------------------------------------------------------------------------------------\n");
        *headerPrinted = true;
    }

    if (curr == NULL) return;

    if (choose == 1) { // preorder
        printRow(curr->nisn, curr->name, curr->born, curr->classes, curr->jurusan, curr->grad);
        viewData(curr->left, choose, headerPrinted);
        viewData(curr->right, choose, headerPrinted);
    } else if (choose == 2) { // inorder
        viewData(curr->left, choose, headerPrinted);
        printRow(curr->nisn, curr->name, curr->born, curr->classes, curr->jurusan, curr->grad);
        viewData(curr->right, choose, headerPrinted);
    } else if (choose == 3) { // postorder
        viewData(curr->left, choose, headerPrinted);
        viewData(curr->right, choose, headerPrinted);
        printRow(curr->nisn, curr->name, curr->born, curr->classes, curr->jurusan, curr->grad);
    } else if (choose == 4){ //choose ipa/ips
    	char jurusan[21];
    	int countMajor = 0;
        
        do{
        	printf("Enter major to search [IPA | IPS]: ");
            gets(jurusan);

            if(strcmp(jurusan, "IPA") != 0 && strcmp(jurusan, "IPS") != 0) {
                printf("Invalid major entered. Please enter 'IPA' or 'IPS'\n");
                continue;
            }

            int countMajor = 0;
            printf("Students with major %s:\n", jurusan);
            viewMajor(root, jurusan, &countMajor);

            if (countMajor == 0) {
                printf("No students found with major %s.\n", jurusan);
            }
		} while(strcmp(jurusan, "IPA") != 0 && strcmp(jurusan, "IPS") !=0);
		
	} else if (choose == 5){ //search nisn
		int nisn;
        int countNISN = 0;
        viewData(root, 2, headerPrinted);
        printf("Enter the 10-digit NISN: ");
        scanf("%10d", &nisn); getchar();
        
        
        searchNISN(root, nisn, &countNISN);
        
        if (countNISN == 0) {
            printf("No students found with NISN %d.\n", nisn);
        }
	}
	
}

bool validateClasses(char classes[]) { //validasi kelas harus 10-12 + A-L
    int length = strlen(classes);

    if (length != 3) return false;

    if (classes[0] != '1') return false;
    if (classes[1] < '0' || classes[1] > '2') return false;

    char huruf = classes[length - 1];
    if (huruf < 'A' || huruf > 'L') return false;

    return true;
}

bool validateBorn(char born[]) { //validasi tanggal lahir supaya dd/mm/yyy, dd 1-30, mm 1-12, yyyy diatas 1990
    if (strlen(born) != 10) return false;

    if (born[2] != '/' || born[5] != '/') return false;

    int day = (born[0] - '0') * 10 + (born[1] - '0');
    int month = (born[3] - '0') * 10 + (born[4] - '0');
    int year = (born[6] - '0') * 1000 + (born[7] - '0') * 100 + (born[8] - '0') * 10 + (born[9] - '0');

    if (day < 1 || day > 30) return false;
    if (month < 1 || month > 12) return false;
    if (year < 1990) return false;

    return true;
}

void insertMenu() { // untuk insert data ke program
    char name[51];
    char classes[21];
    char jurusan[21];
    int nisn;
    char born[41];
    int grad;

    do {
        printf("Input Name [5 s.d 50]: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;
    } while (strlen(name) < 5 || strlen(name) > 50);

    do {
        printf("Input your birth date [dd/mm/yyyy]: ");
        fgets(born, sizeof(born), stdin);
        born[strcspn(born, "\n")] = 0;
    } while (!validateBorn(born));

    do {
        printf("Enter your class [10-12 + A-L] (10B || 11L || 12F): ");
        fgets(classes, sizeof(classes), stdin); //pakai fgets karena gets ada limitnya
        classes[strcspn(classes, "\n")] = 0; //untuk buang buffer di fgets
    } while (!validateClasses(classes));

    do {
        printf("Enter your major [IPA | IPS]: ");
        fgets(jurusan, sizeof(jurusan), stdin);
        jurusan[strcspn(jurusan, "\n")] = 0;
    } while (strcmp(jurusan, "IPA") != 0 && strcmp(jurusan, "IPS") != 0);

    do {
        printf("Enter your graduation year [2006-2024]: ");
        scanf("%d", &grad); getchar();
    } while (grad < 2006 || grad > 2024);

    char yearStr[5];				// untuk buat ID NISN 4 digit awal tahun lahir, 6 sisany random
    strncpy(yearStr, born + 6, 4);
    yearStr[4] = '\0';
    srand(time(NULL)); // supaya tidak terulang angka random nya
    int random = rand() % 1000000;
    char nisnStr[11];
    sprintf(nisnStr, "%s%06d", yearStr, random);
    nisn = atoi(nisnStr); // mengubah string ke int

    root = insertNode(root, name, classes, jurusan, born, nisn, grad); //masukin data ke insert
    printf("Insert Successfully!\n");
    system("pause");
}

void deleteMenu() { //untuk menjalankan deteleting
	bool headerPrinted = false;
	viewData(root, 2, &headerPrinted);
    if(root == NULL) return;

    int nisn;
    printf("Input NISN to Delete: ");
    scanf("%d", &nisn); getchar();

    root = deleteNode(root, nisn);
    if(deleteFound) {
        deleteFound = false;
        printf("Delete successfully\n");
    } else {
        printf("Data not found\n");
    }
    system("pause");
}

int main() { //main code display
    int option = -1;
    while (option != 4) {
    	system("cls");
        printf("================================\n");
        printf("= GRADUATION INFO BINUS SCHOOL =\n");
        printf("================================\n");
        printf("1. Input\n");
        printf("2. View\n");
        printf("3. Delete\n");
        printf("4. Exit\n");
        printf(">> ");
        scanf("%d", &option); getchar();

        switch (option) {
            case 1 : {
                insertMenu();
                break;
            }
            case 2 : {
            	if (root == NULL) {
      				printf("No data available!\n");
   				}
            	else {
            		bool headerPrinted = false;
            		int choose;
            		
        			system ("cls");
        			printf("================================\n");
        			printf("= GRADUATION INFO BINUS SCHOOL =\n");
        			printf("================================\n");
        			printf("Input view graduation :\n");
            		printf("1. View Preorder\n");
            		printf("2. View Inorder\n");
            		printf("3. View Postorder\n");
            		printf("4. View Certain Data\n");
            		printf("5. View Spesific (Search)\n");
            		printf(">> ");
            		scanf("%d", &choose); getchar();
            		switch (choose){
            			case 1 : {
            				viewData(root, 1, &headerPrinted);
							break;
						}
						case 2 : {
							viewData(root, 2, &headerPrinted);
							break;
						}
						case 3 : {
							viewData(root, 3, &headerPrinted);
							break;
						}
						case 4 : {
							viewData(root, 4, &headerPrinted);
							break;
						}
						case 5 : {
							viewData(root, 5, &headerPrinted);
							break;
						}
						default : {
							printf("Input view 1-5 :\n");
							break;
						}
					}	
				}
                system("pause");
                break;
            }
            case 3 : {
                deleteMenu();
                break;
            }
            case 4 : {
                exit(0);
                break;
            }
            default : {
                printf("Invalid Input!\n");
            }
        }
    }

    return 0;
}
