#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#define WIDTH 20
#define HEIGHT 20
#include <locale.h>

int main() {
    int carPos = WIDTH / 2;
    int obstaclePos = rand() % WIDTH;
    int obstaclePos2 = rand() % WIDTH;
    int y = 0;
    int y2 = 0;
    int score = 0;
    int escolha;
    int jogar = 0;
    int delay;
    int speed = 10;
    setlocale(LC_ALL, "Portuguese_Brazil");
    
    do{
    	system("cls");
        printf("=====================================\n");
        printf("        Enduro         \n");
        printf("=====================================\n");
        printf("1. Iniciar jogo\n");
        printf("2. Sair\n");
        printf("Escolha uma opção:\n ");
        scanf("%d", &escolha);
        
    switch (escolha) {
    	case 1:
    		jogar = 1;
    		break;
    	case 2:
    		printf("Saindo...\n");
    		return 0;
    	default:
    		printf("Opção inválida. Pressione uma tecla para tentar de novo...\n");
                getch();
	}
} while (!jogar);
    

    while (1) {
        system("cls");
        delay = 100 - speed * 5;
        if (delay < 10) delay = 10;   // limite mínimo para delay (máxima velocidade)
        if (delay > 100) delay = 100; // limite máximo para delay (velocidade mínima)

        // Desenha a pista
        for (int i = 0; i < HEIGHT; i++) {
    printf("|"); // Borda esquerda

    for (int j = 0; j < WIDTH; j++) {
        if (i == HEIGHT - 1 && j == carPos)
            printf(":__:");  // carro
        else if ((i == y && j == obstaclePos) || (i == y2 && j == obstaclePos2))
            printf(":__:");  // obstáculo
        else
            printf(" ");  // espaço vazio
    }

    printf("|\n"); // Borda direita
}
printf("Pontuação: %d\n", score);

printf("Velocidade atual: %d Km/h aperte w para aumentar e s para diminuir\n", delay);

        // Input
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'a' && carPos > 0)
                carPos--;
            if (ch == 'd' && carPos < WIDTH - 1)
                carPos++;
                if (ch == 'w') {
                if (speed < 20) speed++;  // aumenta velocidade (speed maior -> delay menor)
            }
            if (ch == 's') {
                if (speed > 0) speed--;   // diminui velocidade (speed menor -> delay maior)
            }
            if (ch == 27) // ESC
                break;
        }

        // Atualiza posição do obstáculo
        y++;
        y2++;
        if (y >= HEIGHT) {
            y = 0;
            obstaclePos = rand() % WIDTH;
            score++;
        }
        if (y2 >= HEIGHT) {
            y2 = 0;
            obstaclePos2 = rand() % WIDTH;
            score++;
        }

        // Verifica colisão
        if ((y == HEIGHT - 1 && obstaclePos == carPos) || (y2 == HEIGHT - 1 && obstaclePos2 == carPos)){
		 printf("Bateu! Pontuação final: %d\n", score);
            break;
        }

        Sleep(delay);
    }

    return 0;
}
