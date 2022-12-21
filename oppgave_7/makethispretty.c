//
// Created by marie on 14.12.2022.
//

int main(int argc, char *argv[]) {

    char array[10];
    char secondArray[100];
    int f = 99;

    //1
    for (int i = 0; i < 10; i++) {
        array[i] = '1';
    }

    //2
    for(f; f >= 0; f--){
        secondArray[f] = '2';
    }

    //3 multi-line
    for( int hello = 76; hello <= 100; hello++){
        printf("3 - Hello world!");
        printf("3 - It works with multiple lines");
        printf("3 - And multiple statements");

        //4 - Nested
        for(int k = 0; k < 10; k++){
            printf("4 - And nested statements?");
        }

        printf("3 - And multiple prints");
    }

    //5 - Pointer
    for(int pointer->int = 0; pointer->int < 10; pointer->int++){
        printf("5 - And pointer arguments?");
    }

    //6 - One liner
    for(int j = 0; j < 10; j++){ printf("6 - and one liners?"); }

    //7 - Oneliner before and after forloop
    printf("7- before forloop"); for(int i = 0; i < 10; i++){ printf("7 - hello"); } printf("7 - after forloop");


    //8 - One liner with multiple statements
    for(int j = 0; j < 10; j++){
        printf("8 - and one liners?");
    } printf("8 - with a print after");

    return 0;
}