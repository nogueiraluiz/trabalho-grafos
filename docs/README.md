# Trabalho prático de Teoria dos Grafos - DCC059
Este repositório é destinado ao versionamento do código desenvolvido para os dois trabalhos propostos durante o semestre 2024-1 da disciplina de Teoria dos Grafos do Departamento de Ciência da Computação da Universidade Federal de Juiz de Fora. A disciplina foi ministrada pela Profa. Luciana Brugiolo Gonçalves.
## Integrantes do grupo
Breno Fernandes Brazilino, Igor Mattos da Motta, Luiz Fernando de Melo Nogueira e Nina Aguiar Ferreira.
## Como executar este programa
1. Navegar até a pasta onde o projeto está situado.
2. Compilar o código.
    - Durante o desenvolvimento, compilamos com "$ g++ -o main projeto/\*.cpp".
3. Executar o arquivo gerado com as seguintes opções de argumentos possíveis.
	1. Argumentos comuns a ambas as funcionalidades:
		- $ ./main \<opcaoFuncionalidade> \<caminhoInstancia> \<caminhoSaida>
			- opcaoFuncionalidade: valor inteiro 1 ou 2 sendo 1 para os algoritmos construtivos desenvolvidos como proposta de solução para o MGGPP e 2 para as funcionalidades básicas no contexto de grafos;
			- caminhoInstancia: caminho absoluto ou relativo entre aspas para o arquivo de instância;
			- caminhoSaida: caminho entre aspas no qual deseja-se salvar o arquivo de saída do grafo correspondente à solução construída (caminho relativo ou absoluto).
	1. Argumentos definidos em função de \<opcaoFuncionalidade>:
		1. para \<opcaoFuncionalidade> = 1
			-  $ ./main 1 \<caminhoInstancia> \<caminhoSaida> \<opcaoAlgoritmo> \<alfa>(opcional)
				- opcaoAlgoritmo: 0 para o algoritmo guloso simples, 1 para o randomizado ou 2 para o randomizado reativo;
				- alfa: valor entre 0 e 1 com separador decimal "." (ponto). Este parâmetro será utilizado apenas caso a opção de algoritmo seja o guloso randomizado, com 0 sendo utilizado como padrão caso não seja definido pelo usuário (forçando comportamento igual ao do algoritmo guloso simples)
		2. para \<opcaoFuncionalidade> = 2
			- $ ./main 2 \<caminhoInstancia> \<caminhoSaida> \<direcionado> \<arestasPonderadas> \<verticesPonderados>
				- direcionado: 0 para grafo não direcionado ou 1 para grafo direcionado;
				- arestasPonderadas: 0 para grafo sem ponderação nas arestas ou 1 para grafo com ponderação nas arestas;
				- verticesPonderados: 0 para grafo sem ponderação nos vértices ou 1 para grafo com ponderação nos vértices.
Nota: quaisquer outras combinações de argumentos que não respeitem a forma estabelecida neste documento irão gerar mensagem de erro ou comportamento inesperado.