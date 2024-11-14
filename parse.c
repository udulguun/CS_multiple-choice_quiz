#include <jansson.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "quiz.h"

// Function to decode HTML entities
char *html_entity_decode(const char *str)
{
    size_t len = strlen(str);
    char *decoded_str = malloc(len + 1);
    if (!decoded_str)
        return NULL;

    size_t i = 0, j = 0;
    while (i < len)
    {
        if (str[i] == '&' && str[i + 1] == 'q' && str[i + 2] == 'u' && str[i + 3] == 'o' && str[i + 4] == 't' && str[i + 5] == ';')
        {
            // Decode &quot;
            decoded_str[j++] = '"';
            // Skip over the entity
            i += 6;
        }
        else if (str[i] == '&' && str[i + 1] == 'a' && str[i + 2] == 'p' && str[i + 3] == 'o' && str[i + 4] == 's' && str[i + 5] == ';')
        {
            // Decode &apos;
            decoded_str[j++] = '\'';
            i += 6;
        }
        else if (str[i] == '&' && str[i + 1] == '#' && str[i + 2] == '0' && str[i + 3] == '3' && str[i + 4] == '9' && str[i + 5] == ';')
        {
            // Decode &#039;
            decoded_str[j++] = '\'';
            i += 6;
        }
        else
        {
            // Copy other characters
            decoded_str[j++] = str[i++];
        }
    }
    decoded_str[j] = '\0';
    return decoded_str;
}

int parse(quiz_t *quiz, char *msg)
{
    json_t *root, *results, *question, *correct_answer, *incorrect_answers;
    json_error_t error;

    root = json_loads(msg, 0, &error);
    if (!root)
        return EXIT_FAILURE;

    results = json_object_get(root, "results");
    question = json_object_get(json_array_get(results, 0), "question");
    correct_answer = json_object_get(json_array_get(results, 0), "correct_answer");
    incorrect_answers = json_object_get(json_array_get(results, 0), "incorrect_answers");

    if (!json_is_string(question) || !json_is_string(correct_answer) || !json_is_array(incorrect_answers))
    {
        json_decref(root);
        return EXIT_FAILURE;
    }

    // Decode HTML entities in the question and answer options
    quiz->question = html_entity_decode(json_string_value(question));
    quiz->answer = html_entity_decode(json_string_value(correct_answer));

    // Store the correct answer and the incorrect answers
    quiz->choices[0] = html_entity_decode(json_string_value(correct_answer));
    for (int i = 0; i < 3; i++)
    {
        quiz->choices[i + 1] = html_entity_decode(json_string_value(json_array_get(incorrect_answers, i)));
    }

    // Shuffle the choices to ensure the correct answer isn't always at index 0
    srand(time(NULL));
    for (int i = 0; i < 4; i++)
    {
        // Random index
        int j = rand() % 4;
        // Swap the positions
        char *temp = quiz->choices[i];
        quiz->choices[i] = quiz->choices[j];
        quiz->choices[j] = temp;
    }

    json_decref(root);
    return 0;
}
