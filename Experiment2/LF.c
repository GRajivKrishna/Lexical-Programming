#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Required for exit()

int main() {
    // Variable declarations with more descriptive names
    char production[100];      // To store the full input production (e.g., "abC|abD")
    char part1[50], part2[50];  // To store the two alternatives
    char common_prefix[50];    // To store the identified common prefix
    char modified_gram[50];    // To store the new main production (e.g., "abX")
    char new_gram[50];         // To store the new production for 'X'
    int i = 0, j = 0;
    int prefix_len = 0;

    // --- 1. Get User Input ---
    printf("Enter Production (format A->alpha1|alpha2): A->");
    // Use fgets for safe input to prevent buffer overflow
    fgets(production, sizeof(production), stdin);
    // Remove the newline character that fgets reads
    production[strcspn(production, "\n")] = 0;

    // --- 2. Split the production into two parts based on '|' ---
    char* pipe_pos = strchr(production, '|');
    if (pipe_pos == NULL) {
        printf("\nError: Production must contain a '|' to perform left factoring.\n");
        return 1; // Exit with an error
    }

    // Copy the part before '|' to part1
    strncpy(part1, production, pipe_pos - production);
    part1[pipe_pos - production] = '\0'; // Null-terminate the string

    // Copy the part after '|' to part2
    strcpy(part2, pipe_pos + 1);

    // --- 3. Find the length of the common prefix ---
    while (part1[prefix_len] && part2[prefix_len] && part1[prefix_len] == part2[prefix_len]) {
        prefix_len++;
    }

    // --- 4. Check if a common prefix exists ---
    if (prefix_len == 0) {
        printf("\nNo common prefix found. Grammar remains unchanged:\n");
        printf("A -> %s\n", production);
        return 0;
    }

    // --- 5. Construct the new and modified grammar rules ---
    
    // Copy the common prefix
    strncpy(common_prefix, part1, prefix_len);
    common_prefix[prefix_len] = '\0';

    // Create the modified main production: A -> (common_prefix)X
    sprintf(modified_gram, "%sX", common_prefix);

    // Get the remaining part of part1 (beta1)
    char* rest_of_part1 = part1 + prefix_len;
    // Get the remaining part of part2 (beta2)
    char* rest_of_part2 = part2 + prefix_len;

    // Create the new production for X -> beta1 | beta2
    // Handle the case where one of the remainders is empty (epsilon production)
    if (strlen(rest_of_part1) == 0) {
        // If part1's remainder is empty, use 'ε' (epsilon)
        sprintf(new_gram, "ε|%s", rest_of_part2);
    } else if (strlen(rest_of_part2) == 0) {
        // If part2's remainder is empty, use 'ε'
        sprintf(new_gram, "%s|ε", rest_of_part1);
    } else {
        sprintf(new_gram, "%s|%s", rest_of_part1, rest_of_part2);
    }

    // --- 6. Display the final factored grammar ---
    printf("\n--- After Left Factoring ---\n");
    printf("A -> %s\n", modified_gram);
    printf("X -> %s\n", new_gram);

    return 0;
}
