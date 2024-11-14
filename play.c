#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quiz.h"

int play(quiz_t *quiz)
{
    unsigned attempts = 3;
    unsigned points = 8;
    unsigned decrement = 4;
    unsigned score_awarded = points;
    char input[16];

    printf("%s\n", quiz->question);
    for (int i = 0; i < 4; i++)
    {
        printf("[%c] %s\n", 'a' + i, quiz->choices[i]);
    }

    // Find the index of the correct answer in the shuffled choices
    int correct_index = -1;
    for (int i = 0; i < 4; i++)
    {
        if (strcmp(quiz->choices[i], quiz->answer) == 0)
        {
            correct_index = i;
            break;
        }
    }

    while (attempts > 0)
    {
        printf("(%dpt) > ", score_awarded);
        if (!fgets(input, sizeof(input), stdin))
            return -1;

        input[strcspn(input, "\n")] = 0;

        // Check if the user's answer is correct by comparing to correct_index
        if (input[0] == 'a' + correct_index)
        {
            quiz->score += score_awarded;
            quiz->max += points;
            printf("Congratulations, answer [%c] is correct. Your current score is %d/%d points.\n", input[0], quiz->score, quiz->max);
            return 0;
        }
        else
        {
            printf("Answer [%c] is wrong, try again.\n", input[0]);
            attempts--;
            if (attempts == 1)
            {
                decrement = 2;
            }
            score_awarded -= decrement;
        }
    }
    quiz->max += points;
    return 0;
}
