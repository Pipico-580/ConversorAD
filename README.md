O programa faz uso de várias funcionalidades do Pico, como GPIOs, PWM (Modulação por Largura de Pulso), ADC (Conversor Analógico-Digital), I2C (Comunicação Serial) e interrupções, para criar uma interface interativa com LEDs, um joystick e um display OLED.

Funcionalidades Principais:
GPIOs e LEDs:

O código define pinos GPIO para LEDs RGB (vermelho, verde e azul) e para botões (joystick e botão A).

O LED verde é controlado diretamente por um botão, enquanto os LEDs vermelho e azul são controlados via PWM, permitindo ajustar o brilho com base nos valores do joystick.

ADC e Joystick:

O joystick é conectado a dois canais ADC (X e Y), que leem valores analógicos correspondentes à posição do joystick.

Esses valores são usados para controlar o brilho dos LEDs vermelho e azul (via PWM) e para mover um quadrado na tela do display OLED.

Display OLED (SSD1306):

O display OLED é inicializado via I2C e exibe um quadrado que se move conforme a posição do joystick.

O código também permite alternar entre diferentes bordas no display ao pressionar o botão do joystick.

Interrupções:

O código utiliza interrupções para detectar pressionamentos dos botões. Quando um botão é pressionado, uma função de debounce é chamada para evitar leituras falsas.

O botão do joystick alterna o estado do LED verde e muda a borda do display.

O botão A habilita ou desabilita o controle PWM dos LEDs vermelho e azul.

PWM:

O PWM é configurado para os LEDs vermelho e azul, permitindo que o brilho seja ajustado com base nos valores lidos do joystick.

O brilho é calculado proporcionalmente à distância do joystick em relação ao centro (2048 é o valor central do ADC de 12 bits).

Loop Principal:

No loop principal, o código lê continuamente os valores do joystick, atualiza o brilho dos LEDs (se o PWM estiver ativo) e move o quadrado no display OLED.

O estado do display é atualizado a cada iteração, e os valores do joystick são impressos no console.
