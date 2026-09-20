# Experimento de consumo energético, tempo de resposta e potência

Autores: Ylanna Júlia Soares Alves de Lima e Fernando Oséas Marques de Oliveira
Data: 16 de setembro de 2026

# 1 - INTRODUÇÃO

Em computação, a análise teórica representada pela notação Big-O nos fornece uma estimativa de como os algoritmos se comportam à medida que o volume de dados cresce. No entanto, a teoria muitas vezes ignora as nuances do hardware real: latência da memória RAM, paginação, comportamento do cache do processador e o overhead do sistema operacional.

Este estudo documenta um experimento prático realizado com o objetivo de analisar o impacto do escalonamento de dados no desempenho de diferentes estruturas algorítmicas. O experimento foi executado em um ambiente de hardware legado, porém otimizado por uma distribuição Linux minimalista e focada em performance.

# 2 - AMBIENTE E METODOLOGIA

Os benchmarks foram executados em uma máquina equipada com processador Intel Core i3-3220 (3ª geração) e 8 GB de memória RAM DDR3, sob o sistema operacional Artix Linux. Todos os executáveis foram compilados em C++ no mesmo ambiente para garantir a consistência das medições.

A coleta de métricas foi feita através da ferramenta perf stat, monitorando os seguintes eventos:

- power/energy-pkg/: energia consumida no pacote do processador (medida via registradores RAPL da Intel);
- duration_time: tempo total decorrido (wall time);
- user_time e system_time: tempo de CPU consumido em modo usuário e modo kernel.
- Os valores de energia reportados referem-se estritamente ao pacote do processador, não representando o consumo global do computador ou da fonte de alimentação.

# 3 - IMPLEMENTAÇÃO DOS ALGORITMOS

Os métodos utilizados para os algoritmos clássicos de ordenação incluem o Bolha (Bubble Sort), Seleção (Selection Sort) e Inserção (Insertion Sort). Avaliamos duas versões de cada algoritmo em C++ e coletamos dados de tempo e energia utilizando contadores de eventos do kernel Linux.

Bubble Sort V1 percorre repetidamente o vetor inteiro, comparando elementos vizinhos e trocando-os quando estão fora de ordem. O laço interno continua indo até n-1 em todas as passagens, mantendo O(n²). Bubble Sort V2 usa n-1-i como limite do laço interno. Como os maiores elementos já ficam posicionados no final após cada passagem, as comparações desnecessárias são eliminadas. A complexidade assintótica continua O(n²), mas o trabalho efetivo diminui.

Insertion Sort V1 faz a inserção por meio de trocas consecutivas, então enquanto o elemento estiver menor que o anterior, os dois são trocados. Insertion Sort V2 separa o elemento atual em uma variável temporária e desloca os elementos maiores uma posição para a direita, realizando uma única escrita final. Ambas as versões têm O(n²) no pior caso, mas a V2 tende a realizar menos escritas e movimentações.

Selection Sort V1 procura, para cada posição, o menor elemento restante e faz uma troca ao final da busca. Sua complexidade é O(n²). Selection Sort V2 usa std::priority_queue e insere os valores com sinal invertido para transformar a fila de maior prioridade em uma fila que entrega o menor valor original. As operações de inserção e remoção têm custo O(log n), e como são feitas O(n) operações, o custo total é O(n log n).

# 4 - GERAÇÃO DASENTRADAS E AUTOMAÇÃO

As entradas foram geradas pelo programa geraEntrada.cpp, que cria uma sequência de 1 a n e realiza 2n trocas aleatórias de posição usando rand(). Foram gerados arquivos para 10.000, 50.000 e 100.000 elementos.

A execução foi automatizada e feita por scripts em Bash. Para mitigar ruídos do sistema operacional, cada combinação de algoritmo e tamanho foi executada 10 vezes consecutivas, com redirecionamento das métricas do perf para um arquivo resultado_i.txt. A repetição reduz a dependência de uma única execução e permite observar a variabilidade.

# 5 - INSTRUMENTAÇÃO E MÉTRICAS

A ferramenta utilizada foi o perf stat, com power/energy-pkg/, duration_time, user_time e system_time. Cada combinação de algoritmo e tamanho teve dez execuções. A planilha dos resultados mantém as medições individuais e as médias usadas.

O duration_time representa o tempo decorrido da execução observada. O user_time mede o tempo de CPU gasto em espaço de usuário, enquanto system_time representa o tempo de CPU gasto em chamadas ao sistema e a planilha consolida essas informações em uma coluna de tempo de CPU. A energia é reportada em joules.

# 6 - RESULTADOS

As tabelas abaixo resume as médias das dez execuções por algoritmo e tamanho.

Bolha
N	Energia	Tempo Exec. (ms)	Tempo CPU (ms)	Potência Exec.	Potência CPU
10.000	5,807	351,9	372	0,0165	0,0157
50.000	155,268	9.346,7	9.937	0,0166	0,0156
100.000	627,578	37.700	39.786	0,0166	0,0158
Bolha V2
N	Energia	Tempo Exec. (ms)	Tempo CPU (ms)	Potência Exec.	Potência CPU
10.000	4,16	244,3	267	0,0170	0,0158
50.000	112,755	6.740,8	7.122	0,0167	0,0158
100.000	460,63	27.362,2	28.847	0,0168	0,0160

Nos 10.000 elementos, Bubble V1 consumiu 5,807 J contra 4,160 J da V2, uma redução de 28,4%. Em 50.000 elementos, a redução foi de 27,4%, e em 100.000, de 26,6%. O ganho é consistente porque a V2 elimina comparações no final de cada passagem, mas continua sendo um algoritmo quadrático.

Seleção
N	Energia	Tempo Exec. (ms)	Tempo CPU (ms)	Potência Exec.	Potência CPU
10.000	1,658	106,1	111	0,0156	0,0151
50.000	39,675	2.457,1	2.612	0,0161	0,0152
100.000	157,669	9.772,7	10.334	0,0161	0,0153
Seleção V2
N	Energia	Tempo Exec. (ms)	Tempo CPU (ms)	Potência Exec.	Potência CPU
10.000	0,169	10,7	14	0,0161	0,0135
50.000	0,90	49,4	72	0,0182	0,0132
100.000	1,779	98,1	134,6	0,0181	0,0135

No Selection Sort, a V1 consumiu 1,658 J, 39,675 J e 157,669 J, enquanto a V2 consumiu 0,169 J, 0,900 J e 1,779 J. As reduções correspondentes são 89,8%, 97,7% e 98,9%. O tempo acompanha o mesmo fenômeno, para 100.000 elementos, 9.772,7 ms na V1 contra 98,1 ms na V2. O aumento de dez vezes no tamanho da entrada produz crescimento próximo de cem vezes na versão O(n²), mas aproximadamente dez vezes na versão O(n log n).

Inserção
N	Energia	Tempo Exec. (ms)	Tempo CPU (ms)	Potência Exec.	Potência CPU
10.000	1,719	105,2	124	0,0163	0,0148
50.000	41,862	2.544,2	2.711	0,0165	0,0154
100.000	166,741	10.135,4	10.708	0,0165	0,0156
Inserção V2
N	Energia	Tempo Exec. (ms)	Tempo CPU (ms)	Potência Exec.	Potência CPU
10.000	1,021	63,6	65	0,0161	0,0159
50.000	24,693	1.507,5	1.625	0,0164	0,0152
100.000	97,949	5.980	6.354	0,0164	0,0154

No Insertion Sort, a diferença foi maior: 40,6%, 41,0% e 41,3% de redução energética para 10.000, 50.000 e 100.000 elementos. O padrão acompanha a mudança de implementação, em vez de trocar repetidamente pares adjacentes, a V2 desloca uma sequência e realiza uma colocação final. Então a classe assintótica não muda, mas a quantidade de movimentações diminui.

Ainda é possível observar o comportamento da potência de execução e de CPU em cada método. No Bubble Sort a potência de execução permanece constante nas duas versões (0,0165-0,0170 W) e a mesma coisa acontece com a potência de CPU (0,0157-0,0160 W). Sendo assim, a redução de energia da V2 está principalmente associada à diminuição do tempo de execução e não de uma redução significativa da potência instantânea utilizada. A potência de execução permanece próxima de 0,0161–0,0165 W nas duas versões, enquanto a potência de CPU varia aproximadamente entre 0,0148 e 0,0159 W. Novamente, o principal fator para a economia de energia é a redução do tempo necessário para concluir a execução.

O Insertion Sort se comporta semelhante ao Bubble Sort.A potência de execução permanece próxima de 0,0161–0,0165 W nas duas versões, enquanto a potência de CPU varia aproximadamente entre 0,0148 e 0,0159 W. Novamente, o principal fator para a economia de energia é a redução do tempo necessário para concluir a execução.

No Selection Sort se tem um comportamento um pouco diferente. Na versão base que foi usada (V1), a potência de execução fica entre 0,0156 - 0,0161 W, enquanto na versão dois (2) a potência fica entre 0,0181-0,0182 W. Apesar do aumento, a segunda versão apresenta uma redução muito expressiva no consumo energético e isso ocorre por causa do tempo de execução ser mais baixo. Desse modo, os resultados mostram que uma potência média ligeiramente maior não implica necessariamente maior consumo energético.

# 7 - CONSIDERAÇÕES FINAIS

Este estudo demonstrou experimentalmente a relação direta entre a eficiência algorítmica, o tempo de execução e o consumo energético do processador. Dado os resultados, é possível confirmar que a otimização no momento de construção de um código (mesmo que minimamente) é uma parte importante na área da sustentabilidade na tecnologia da informação.

Primeiramente, foi possível observar que ao mudar o grau de complexidade, se obtém uma redução no consumo energético, como mostrado na substituição de O(n²) para uma versão O(n log n) do Selection Sort, que reduziu o consumo de energia em até 98% para grandes valores de dados.

Segundamente, os dados mostraram a dependência do consumo energético em relação ao tempo de execução. Desse modo, um algoritmo que opera sob uma potência média levemente maior  mostra-se mais sustentável quando realiza um trabalho em um tempo menor.

Por fim, embora as economias de energia observadas em um único computador pareçam modestas, a aplicação dessas práticas de otimização em longa escala resulta em um impacto na redução de demanda elétrica global. Assim, é possível concluir que a sustentabilidade energética deve destacar-se como critério essencial em todas as etapas de desenvolvimento de software.

