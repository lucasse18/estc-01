#!/bin/bash
set -e

# parametros
# ----------
SAMPLES="${1}"
MEANS="${2}"
P="${3}"

# definicao de funcoes
# --------------------
die() {
  # imprime seus argumentos na saida padrao de erros (2) e executa 'exit 1'
  echo "[ERROR]: ${@}" >&2;
  exit 1;
}

check() {
  # verifica se seu argumento e' um executavel reconhecido pela shell
  type -a ${@} > /dev/null 2>&1 || die "${@} nao encontrado no path."
}

plot() {
  COLOR="#516288"
  DATA="./data/bernoulli.csv"

  gnuplot << EndOfFile
  set datafile separator ","
  set title "Médias amostrais para Bernoulli(${P})"

  set terminal svg enhanced size 400,400 background rgb 'white'
  set output "plot.svg"

  stats '${DATA}' using 1 name 'stats' nooutput

  set bmargin 3
  set tmargin 2
  set xlabel "Valor da média"
  set ylabel "Número da média"
  set grid
  set key right bottom Right font ",10" spacing 0.7

  set style line 1 lc rgb '${COLOR}' pt 7 ps 0.5
  plot '${DATA}' ls 1 t 'média amostral',\
  stats_median ls 1 lc rgb 'grey' title 'mediana'

EndOfFile
}


# "entry point" do script (a execucao comeca aqui)
# ------------------------------------------------
if [[ ${4} != "plot" ]]; then
  check make

  # tenta executar o Makefile do projeto
  make || die "make retornou codigo ${?}."

  # cria o diretorio data caso nao exista
  [ -d ./data ] || mkdir ./data

  ./bin/bernoulli -o ./data/bernoulli.csv -m ${1} -n ${2} ${3}
fi

check gnuplot
plot
