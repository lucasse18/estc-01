SHELL   = /bin/bash
CC      = gcc
CFLAGS  = -std=gnu99 -O2 -fomit-frame-pointer -Wall

OUTDIR  = ./bin
SRCDIR  = ./src
INCDIR  = ./include
TARGETS = $(addprefix ${OUTDIR}/,bernoulli)

.PHONY: all
all: ${TARGETS}

${OUTDIR}/%: ${SRCDIR}/main.c ${INCDIR}/dbg.h ${SRCDIR}/%.c ${INCDIR}/%.h
	${CC} ${CFLAGS} -I${INCDIR} -o $@ $^

${TARGETS}: | ${OUTDIR}

${OUTDIR}:; [ -d $@ ] ||  mkdir $@

.PHONY: clean
clean:; [ -d ${OUTDIR} ] && rm -rf ${OUTDIR}
