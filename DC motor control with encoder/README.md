# ARM_Projects
DC motor speed control using a rotary incremental encoder
#
Menu serial onde o usuário pode controlar a velocidade de um motor DC inserindo um setpoint em RPM ou simplesmente aumentando/diminuindo a porcentagem do PWM. No menu, o usuário pode também visualizar informações como a velocidade, as rotações e o ângulo do motor.  Para alcançar isso foi feito um controle em malha fechada com um encoder incremental óptico. A saída digital do encoder é ligada no ARM que, usando os periféricos do TIM, calcula o RPM do motor e compara com a velocidade esperada (setpoint). Através da técnica PID, o programa consegue usar essas informações para manter o motor com o RPM o mais próximo possível do desejado.
