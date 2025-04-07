#!/bin/bash

MINISHELL=./minishell
REF_SHELL=/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
RESET='\033[0m'

test_cases=(
  # === BUILTINS ===
  'echo hello world'
  'echo -n "no newline"'
  'pwd'
  'export FOO=bar'
  'echo $FOO'
  'unset FOO'
  'echo $FOO'
  'env | grep PATH'

  # === CD ===
  'pwd'
  'cd /'
  'pwd'
  'cd -'  # may fail if OLDPWD unset

  # === PIPES ===
  'echo hello | cat'
  'ls | grep minishell'

  # === REDIRECTIONS ===
  'echo test > out.txt'
  'cat < out.txt'
  'echo more >> out.txt'
  'cat < out.txt'
  'rm out.txt'

  # === QUOTES ===
  'echo "double quoted $USER"'
  "echo 'single quoted $USER'"
  "echo 'nested \"quote\" test'"

  # === VARIABLES ===
  'export VAR=hello'
  'echo $VAR'
  'unset VAR'
  'echo $VAR'

  # === SYNTAX EXIT_FAILURES ===
  '|'
  'echo hello | | cat'
  'cat <'
  'echo >'

  # === EXIT ===
  'exit'
)

filter_output() {
  sed '/^minishell\$ /d' |          # プロンプト行そのものを削除
  sed 's/minishell\$//g' |          # 行末にくっついた minishell$ を削除
  sed '/^exit$/d' |                 # exit 単体行を削除
  sed '/^ *$/d'                     # 空白だけの行も削除（任意）
}

run_test() {
  local input="$1"
  local expected result

  expected=$(echo "$input" | $REF_SHELL 2>&1 | filter_output)
  result=$(echo "$input" | $MINISHELL 2>&1 | filter_output)

  if [ "$expected" = "$result" ]; then
    echo -e "✅ ${GREEN}$input${RESET}"
  else
    echo -e "❌ ${RED}$input${RESET}"
    echo "   expected: $expected"
    echo "   got     : $result"
  fi
}


echo "🚀 Running minishell tester..."

for test in "${test_cases[@]}"; do
  run_test "$test"
done
echo "✅ All tests completed."