#include <stdlib.h>
int leastInterval(char* tasks, int tasksSize, int n) {
    int counts[26] = {0};
    for (int i = 0; i < tasksSize; i++) {
        counts[tasks[i] - 'A']++;
    }
    int max_freq = 0;
    for (int i = 0; i < 26; i++) {
        if (counts[i] > max_freq) {
            max_freq = counts[i];
        }
    }
    int max_freq_count = 0;
    for (int i = 0; i < 26; i++) {
        if (counts[i] == max_freq) {
            max_freq_count++;
        }
    }
    int calculated_chunks = (max_freq - 1) * (n + 1) + max_freq_count;
    return (tasksSize > calculated_chunks) ? tasksSize : calculated_chunks;
}
