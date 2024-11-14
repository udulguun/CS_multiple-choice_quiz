#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quiz.h"

int main()
{
    quiz_t quiz = {0, 0, 0, NULL, NULL, {NULL}};
    char *url = "https://opentdb.com/api.php?amount=1&category=18&type=multiple";

    printf("Answer multiple choice questions about computer science.\n");

    while (1)
    {
        char *response = fetch(url);
        if (!response)
            break;

        if (parse(&quiz, response) == -1)
        {
            free(response);
            continue;
        }

        if (play(&quiz) == -1)
            break;

        free(quiz.question);
        free(quiz.answer);
        for (int i = 0; i < 4; i++)
        {
            free(quiz.choices[i]);
        }
        free(response);
    }

    printf("\nThanks for playing today.\n");
    printf("Your final score is %d/%d points.\n", quiz.score, quiz.max);
    return EXIT_SUCCESS;
}
