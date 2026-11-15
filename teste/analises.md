## Testes do Sistema de Gestão de Estacionamento

Foram realizados testes para verificar o funcionamento das principais funcionalidades do sistema, incluindo cadastro, validação de dados, atualização, movimentação e remoção de veículos.

### 1. Cadastro de veículos
- Cadastro realizado com sucesso utilizando identificador válido.
- Cadastro recusado ao tentar utilizar uma vaga ocupada.
- Cadastro recusado ao informar um identificador já existente.
- Validação de identificador com exatamente 7 caracteres alfanuméricos.
- Validação de categoria: carro (1) ou moto (2).
- Validação de posição: fileiras de 1 a 4 e colunas de 1 a 5.

### 2. Atualização e movimentação
- Atualização da descrição e categoria de um veículo.
- Movimentação de veículo para outra vaga disponível.
- Remoção de veículo com liberação da vaga correspondente.

### 3. Listagem e ocupação
- Listagem dos veículos cadastrados com identificador, categoria, vaga e descrição.
- Exibição do mapa de vagas, identificando posições ocupadas e livres.
- Geração de relatório com quantidade de carros, motos e percentual de ocupação.

### 4. Teste de capacidade máxima
- Cadastro de veículos até atingir o limite de 20 vagas.
- Tentativa de cadastro adicional recusada por estacionamento lotado.
- Relatório final confirmando 20 vagas ocupadas e 100% de ocupação.

### Resultado dos testes

**<text>Todos os principais cenários testados apresentaram os resultados esperados.</text>**

- Capacidade total: 20 vagas.
- Veículos cadastrados: 20.
- Carros: 9.
- Motos: 11.
- Ocupação: 100%.
