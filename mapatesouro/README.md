# FIAP - Faculdade de Informática e Administração Paulista

<p align="center">
<a href= "https://www.fiap.com.br/"><img src="assets/logo-fiap.png" alt="FIAP - Faculdade de Informática e Admnistração Paulista" border="0" width=40% height=40%></a>
</p>

<br>

# FarmTech Solutions

## IA_Underground

## 👨‍🎓 Integrantes: 
- <a href="https://www.linkedin.com/company/inova-fusca">Danilo Marques Dantas</a>
- <a href="https://www.linkedin.com/in/marlonmarinho/">Marlon Paulino Marinho</a>
- <a href="https://www.linkedin.com/in/pedro-carvalho-cea-149658137/">Pedro Carvalho Rocha Lima</a> 
- <a href="https://www.linkedin.com/in/viniciusl7">Vinicius Lisboa Porto</a> 
- <a href="https://www.linkedin.com/in/vinigama">Vinicius de Santana Gama</a>

## 📜 Descrição
O objetivo deste projeto é simular um sistema de irrigação automatizado que decide quando acionar uma bomba d'água com base em dados de múltiplos sensores. O sistema monitora em tempo real a umidade do solo, o pH e os níveis de nutrientes essenciais (Nitrogênio, Fósforo e Potássio - NPK).
Toda a lógica foi implementada em C++ no framework Arduino e testada no simulador Wokwi, que permite a prototipagem de projetos com o ESP32 sem a necessidade do hardware físico.

<p align="center">
<img src="mapatesouro.png" border="0" width=30% height=30%>
</p>


## 📁 Estrutura de pastas

<b>diagram.json:</b> Define o circuito eletrônico para a simulação online no Wokwi. Ele lista os componentes de hardware (como o ESP32, sensores e botões) e como eles estão conectados entre si. <br>

<b>platformio.ini:</b> É o arquivo de configuração principal para o PlatformIO. Ele especifica a placa utilizada (ESP32), o framework de programação (Arduino) e as bibliotecas necessárias para o projeto.<br>

<b>.gitignore:</b> Indica ao sistema de controle de versão Git quais arquivos e pastas devem ser ignorados. Isso evita que arquivos temporários ou de configuração local sejam enviados para o repositório.<br>

<b>wokwi.toml:</b> Arquivo de configuração que conecta o PlatformIO ao simulador Wokwi. Ele informa ao Wokwi onde encontrar o firmware compilado do projeto para que a simulação possa ser executada.<br>

<b>README.md:</b> É o documento principal do projeto, contendo a descrição geral, a lista de integrantes, a estrutura dos diretórios e as instruções de como executar o código.<br>

## 🔧 Como executar o código
Para rodar esta simulação, siga os passos abaixo: <br>

<b>1 - Pré-requisitos:</b> <br>
Ter o Visual Studio Code instalado.<br>
Ter a extensão PlatformIO IDE instalada no VS Code. <br>

<b>Configuração:</b> <br>
Clone ou faça o download deste repositório.<br>
Abra a pasta do projeto no Visual Studio Code. O PlatformIO irá detectar o arquivo platformio.ini e instalará automaticamente as bibliotecas necessárias (Adafruit Unified Sensor e DHT sensor library).<br>

<b>Execução:</b> <br>
Com o projeto aberto, pressione F1 e procure por PlatformIO: Start Simulation. <br>
O Wokwi abrirá em uma aba do VS Code ou no seu navegador, com o circuito montado e o código pronto para ser executado.<br>
Inicie a simulação e interaja com os sensores para testar a lógica de irrigação.



## 🗃 Histórico de lançamentos

* 0.1.0 - 14/10/2025
  
