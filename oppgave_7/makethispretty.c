//
// Created by marie on 14.12.2022.
//

int main(int argc, char *argv[]) {

    char array[10];
    char secondArray[100];
    int f = 99;

    for (int i = 0; i < 10; i++) {
        array[i] = 'a';
    }

    for(f; f >= 0; f--){
        secondArray[f] = 'b';
    }

    for( int hello = 76; hello <= 100; hello++){
        printf("Hello world!");
        printf("It works with multiple lines");
        printf("And multiple statements");

        for(int k = 0; k < 10; k++){
            printf("And nested statements?");
        }

        printf("And multiple prints");
    }

    for(int pointer->int = 0; pointer->int < 10; pointer->int++){
        printf("And pointer arguments?");
    }

    for(int j = 0; j < 10; j++){ printf("and one liners?"); }

    printf("before forloop"); for(int i = 0; i < 10; i++){ printf("hello"); }

    return 0;
}