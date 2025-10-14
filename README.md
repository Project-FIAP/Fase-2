# FIAP - Faculdade de Informática e Administração Paulista

<p align="center">
<a href= "https://www.fiap.com.br/"><img src="RepositorioFiap/templateFiapVfinal-main/assets/logo-fiap.png" alt="FIAP - Faculdade de Informática e Admnistração Paulista" border="0" width=40% height=40%></a>
</p>

<br>

# FarmTech

## Nome do grupo

## 👨‍🎓 Integrantes: 
- <a href="https://www.linkedin.com/company/inova-fusca">Danilo Marques Dantas</a>
- <a href="https://www.linkedin.com/company/inova-fusca">Marlon Paulino Marinho</a>
- <a href="https://www.linkedin.com/company/inova-fusca">Pedro Carvalho Rocha Lima</a> 
- <a href="https://www.linkedin.com/company/inova-fusca">Vinicius Lisboa Porto</a> 
- <a href="https://www.linkedin.com/in/vinigama">Vinicius de Santana Gama</a>

## 👩‍🏫 Professores:
### Tutor(a) 
- <a href="https://www.linkedin.com/company/inova-fusca">Nome do Tutor</a>
### Coordenador(a)
- <a href="https://www.linkedin.com/company/inova-fusca">Nome do Coordenador</a>


## 📜 Descrição

Este projeto foi desenvolvido no âmbito do Problem Based Learning (PBL) com o tema Agronegócio, e consiste em uma solução de software para o gerenciamento de insumos agrícolas, com foco específico no controle de estoque e validade de sementes.

O agronegócio é um pilar fundamental da economia brasileira, um setor que vai muito além da fazenda e que hoje é sinônimo de inovação e tecnologia (AgroTech). No entanto, um dos desafios enfrentados por pequenos e médios produtores é a gestão eficiente de seus insumos. Perdas de sementes por vencimento, falta de controle sobre a quantidade disponível e dificuldade no planejamento de safras são problemas reais que impactam diretamente a produtividade e a lucratividade.

Visando solucionar essa "dor", este sistema oferece uma ferramenta simples e funcional, desenvolvida em Python e operada via terminal (interface de linha de comando). A aplicação se conecta a um banco de dados Oracle para garantir a persistência e a segurança dos dados, permitindo ao produtor realizar as seguintes operações:

<b>Cadastrar:</b> Adicionar novos tipos de sementes ao estoque, com informações de nome, quantidade e data de validade.

<b>Listar:</b> Visualizar de forma organizada todas as sementes em estoque, com um sistema de status que alerta sobre a proximidade do vencimento.

<b>Editar e Excluir:</b> Atualizar informações de sementes existentes ou remover itens do estoque.

<b>Buscar:</b> Pesquisar rapidamente por uma semente específica pelo nome.

<b>Verificar Validade:</b> Gerar um relatório focado em sementes que estão vencidas ou que irão vencer nos próximos 30 dias.

O projeto materializa o conceito de AgroTech ao aplicar tecnologia para resolver um problema prático do campo, oferecendo uma solução robusta para otimizar o controle de insumos, reduzir desperdícios e auxiliar na tomada de decisões estratégicas do produtor rural.


## 📁 Estrutura de pastas

Dentre os arquivos e pastas presentes na raiz do projeto, definem-se:

- <b>.github</b>: Nesta pasta ficarão os arquivos de configuração específicos do GitHub que ajudam a gerenciar e automatizar processos no repositório.

- <b>assets</b>: aqui estão os arquivos relacionados a elementos não-estruturados deste repositório, como imagens.

- <b>config</b>: Posicione aqui arquivos de configuração que são usados para definir parâmetros e ajustes do projeto.

- <b>document</b>: aqui estão todos os documentos do projeto que as atividades poderão pedir. Na subpasta "other", adicione documentos complementares e menos importantes.

- <b>scripts</b>: Posicione aqui scripts auxiliares para tarefas específicas do seu projeto. Exemplo: deploy, migrações de banco de dados, backups.

- <b>src</b>: Todo o código fonte criado para o desenvolvimento do projeto ao longo das 7 fases.

- <b>README.md</b>: arquivo que serve como guia e explicação geral sobre o projeto (o mesmo que você está lendo agora).

## 🔧 Como executar o código

Pré-requisitos
Antes de começar, garanta que você tenha os seguintes softwares instalados:

Python: Versão 3.10 ou superior.

Git: Para clonar o repositório.

Acesso a um Banco de Dados Oracle: O sistema foi configurado para se conectar a uma instância Oracle. As credenciais e o endereço do banco precisam ser configurados.

Instalação e Configuração
Clone o repositório:
Abra seu terminal e execute o seguinte comando:

Bash

git clone https://github.com/Project-FIAP/Fase-2.git
Navegue até a pasta do projeto:

Bash

cd Fase-2/python_e_alem
Instale as dependências:
O projeto requer a biblioteca oracledb para se comunicar com o banco de dados. Instale-a usando o pip:

Bash

pip install oracledb
Configure o Banco de Dados:
A. Crie a tabela de sementes na sua instância Oracle executando o seguinte script SQL:

SQL

CREATE TABLE TBL_SEMENTES (
    ID_SEMENTE      NUMBER GENERATED AS IDENTITY PRIMARY KEY,
    NOME            VARCHAR2(100) NOT NULL,
    QUANTIDADE      NUMBER(10) NOT NULL,
    DT_VALIDADE     DATE,
    CONSTRAINT chk_quantidade_positiva CHECK (QUANTIDADE >= 0)
);
B. Abra o arquivo database.py e atualize as variáveis user, password e dsn com as suas credenciais de acesso ao banco de dados Oracle.

Execução
Após a configuração, execute o programa principal a partir do terminal, na pasta do projeto:

Bash

python "#Gerenciamento de Sementes.py"
O menu interativo da aplicação será exibido no terminal, e você poderá começar a usar o sistema.


## 🗃 Histórico de lançamentos

* 0.1.0 - 14/10/2024
  

## 📋 Licença

<img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/cc.svg?ref=chooser-v1"><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/by.svg?ref=chooser-v1"><p xmlns:cc="http://creativecommons.org/ns#" xmlns:dct="http://purl.org/dc/terms/"><a property="dct:title" rel="cc:attributionURL" href="https://github.com/agodoi/template">MODELO GIT FIAP</a> por <a rel="cc:attributionURL dct:creator" property="cc:attributionName" href="https://fiap.com.br">Fiap</a> está licenciado sobre <a href="http://creativecommons.org/licenses/by/4.0/?ref=chooser-v1" target="_blank" rel="license noopener noreferrer" style="display:inline-block;">Attribution 4.0 International</a>.</p>

