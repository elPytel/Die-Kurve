#!/usr/bin/env bash

set -euo pipefail

if [ "$#" -ne 2 ]; then
    echo "Použití: $0 <složka_s_testy> <výstupní_runner.c>"
    exit 1
fi

TESTS_DIR="$1"
OUTPUT_RUNNER="$2"

# Vyčištění/vytvoření výstupního souboru s nízkoúrovňovou zachytávací logikou
cat << 'EOF' > "$OUTPUT_RUNNER"
/* Automaticky generovaný master test runner - puristické C & assert */
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>

int tests_passed = 0;
int tests_failed = 0;
jmp_buf env;

void handle_assert(int sig) {
    tests_failed++;
    printf("\033[0;31m[ FAILED ]\033[0m\n");
    longjmp(env, 1);
}
EOF

echo -e "\n/* Prototypy všech nalezených testů v projektu */" >> "$OUTPUT_RUNNER"

# 1. Najdeme VŠECHNY funkce void test_* napříč všemi soubory ve složce tests/
# Využijeme subshell, aby nám případný neúspěch grep (když nic nenajde) neshodil set -e
ALL_TESTS=$(grep -rhoE "void test_[a-zA-Z0-9_]+" "$TESTS_DIR" 2>/dev/null || true)

if [ -z "$ALL_TESTS" ]; then
    echo "Varování: V adresáři $TESTS_DIR nebyly nalezeny žádné testovací funkce."
fi

# Zapíšeme prototypy do hlavičky generovaného souboru
echo "$ALL_TESTS" | sort -u | sed 's/$/(void);/' >> "$OUTPUT_RUNNER"

cat << 'EOF' >> "$OUTPUT_RUNNER"

int main(void) {
    signal(SIGABRT, handle_assert);
    printf("\n\033[1;36m=== Running Purist Automated Test Suite ===\033[0m\n\n");
EOF

# 2. Vygenerujeme spouštěcí sekvence pro každou unifikovanou funkci
echo "$ALL_TESTS" | grep -oE "test_[a-zA-Z0-9_]+" | sort -u | while read -r test_name; do
    cat << EOF >> "$OUTPUT_RUNNER"
    printf("Running %-45s ", "${test_name}...");
    fflush(stdout);
    if (setjmp(env) == 0) {
        ${test_name}();
        tests_passed++;
        printf("\033[0;32m[ PASSED ]\033[0m\n");
    }
EOF
done

# 3. Barevný sumář
cat << 'EOF' >> "$OUTPUT_RUNNER"
    printf("\n--------------------------------------------------\n");
    if (tests_failed > 0) {
        printf("\033[1;31mFAILURES DETECTED! Passed: %d | Failed: %d\033[0m\n\n", tests_passed, tests_failed);
        return 1;
    } else {
        printf("\033[1;32mSUCCESS! All %d tests passed cleanly.\033[0m\n\n", tests_passed);
        return 0;
    }
}
EOF