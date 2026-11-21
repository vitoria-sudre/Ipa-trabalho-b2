### Testes do Sistema de Gestão de Estacionamento

Foram realizados testes manuais no terminal para verificar o funcionamento das principais funcionalidades do sistema, incluindo cadastro, validação de dados, atualização, movimentação, remoção, relatório e gerenciamento de memória. Os casos detalhados, com as saídas do terminal, estão na seção 7 do `README.md`.

### 1. Estacionamento vazio
- Listagem vazia com mensagem específica.
- Mapa com as 20 vagas livres.
- Relatório com 0 vagas ocupadas e ocupação de 0,00%.

### 2. Cadastro de veículos
- Cadastro realizado com sucesso utilizando identificador válido.
- Cadastro recusado ao tentar utilizar uma vaga ocupada.
- Cadastro recusado ao informar um identificador já existente, inclusive com letras minúsculas (`bbb0002` reconhecido como `BBB0002`).
- Identificador em minúsculas convertido para maiúsculas (`ccc0976` cadastrado como `CCC0976`).
- Validação de identificador com exatamente 7 caracteres alfanuméricos (identificador com espaço no final recusado).
- Validação de categoria: carro (1) ou moto (2).
- Validação de posição: fileiras de 1 a 4 e colunas de 1 a 5.

### 3. Crescimento do vetor dinâmico
- Capacidade inicial de 2 registros.
- Ao cadastrar o terceiro veículo, a capacidade foi ampliada com `realloc` (2 para 4), preservando os registros anteriores.

### 4. Atualização, movimentação e remoção
- Atualização da descrição e da categoria de um veículo, sem duplicar o registro.
- Movimentação de veículo para outra vaga disponível, liberando a vaga de origem.
- Remoção de veículo com liberação da vaga correspondente, sem aparecer na listagem seguinte.

### 5. Listagem e ocupação
- Listagem dos veículos cadastrados com identificador, categoria, vaga e descrição.
- Exibição do mapa de vagas, identificando posições ocupadas e livres.
- Geração de relatório com quantidade de carros, motos e percentual de ocupação.

### 6. Teste de capacidade máxima
- Cadastro de veículos até atingir o limite de 20 vagas.
- Tentativa de cadastro adicional recusada por estacionamento lotado.
- Relatório final confirmando 20 vagas ocupadas e 100% de ocupação.

### 7. Encerramento
- Ao escolher a opção 0, o programa libera a memória do vetor dinâmico e encerra normalmente.

### Resultado dos testes

**Todos os principais cenários testados apresentaram os resultados esperados.**

- Capacidade total: 20 vagas.
- Veículos cadastrados: 20.
- Carros: 9.
- Motos: 11.
- Ocupação: 100%.
