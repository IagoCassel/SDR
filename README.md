# SDR

## Descrição do projeto:

O propósito desse projeto é a produção da comunicação de hardware comumente usado na **eletrônica**, com o **ATmega 328P**, para produção de um Software-defined Radio (Radio Definido por Software). Esse dispositivo tem como papel realizar a demodulação AM e FM de sinais recebidos por um canal único do ATmega. A ideia é tentar realizar por meio de software o tratamento do sinal, de forma a usar conhecimentos prévios de **Processamento Digital de Sinais**. O projeto foi desenvolvido com o objetivo de ser **eficiente**, utilizando o mínimo de recurso de hardware possível, para **redução de custos** de uma possível produção mercadológica. De maneira didática, o SDR foi responsável por produzir uma **interlocução entre a produção de um sistema embarcado com a disciplina de processamento de dados digitais**, fomentar o conhecimento em baixo nível das ferramentas comuns do meio da **Engenharia Mecatrônica e Automação** e criar uma interlocução entre a eletrônica, Software em baixo nível e limitações mercadológicas.

Esse projeto foi feito em conjunto com outros integrantes. Para maior detalhamento acesse o **Relatorio.pdf** desse repositório. Por fim, **script.c** apresenta o código em C que atua no ATmega 328P. O arquivo **SDR.pdsprj** contem o arquivo do projeto no Proteus.

## Habilidades adquiridas:

A produção desse projeto possibilitou o aprendizado de:
* Capacidade de liderança em projetos problemáticos;
* Conhecimento específico do ATmega 328P; 
* Reprodução, por Software, de demodulações comumente usadas para sinais analógicos;
* Tratamento de sinal para economia na utilização de canais únicos de comunicação;
* Leitura e entendimento de datasheets para produção de projetos;
* Reconhecimento de erros teóricos em planejamentos;
* Capacidade de avaliação crítica na alocação e utilização de recursos.

## Problemas encontrados:

Como visto no **Relatorio.pdf**, apesar de conseguir produzir um comportamento adequado para seu projeto RTL e o código ser capaz de aplicar a demodulação de sinais digitalizados, o projeto não conseguiu funcionar de maneira satisfatória ao final do prazo. Entretanto, foi verificado que a causa desse empasse foi a má utilização do tempo de amostráem, junto às interrupções do ATmega, por discuido na análise teórica. Esse empasse fez com que o sistema como um todo não retornasse uma resposta satisfatória, por mais que suas partes funcionassem de forma coerente. Essa percepção serve de experiência para próximos projetos e ponto de partida para update desse. 

## Elementos Homem-Máquina do SDR:

<p align="center">
  <img src="https://user-images.githubusercontent.com/48588172/134427725-13e35f03-2f13-4560-8ef7-21f78a1ea6d4.png" />
</p>

## Hardware produzido:

<p align="center">
  <img src="https://user-images.githubusercontent.com/48588172/134427989-c19a2bfe-911f-4478-95fa-c7c86824609e.png" />
</p>

## Demodulação AM por Software:

<p align="center">
  <img src="https://user-images.githubusercontent.com/48588172/134427744-d5eb53e7-9c72-4452-ad22-77a7d7031f73.png" />
</p>

## Demodulação ASK por Software:

<p align="center">
  <img src="https://user-images.githubusercontent.com/48588172/134427754-9c0c2555-2a32-4320-b315-9b3ae65bc03e.png" />
</p>
