(define (problem indoparaobrt)
	(:domain caminhando)
	(:objects S7 CorredorS Escada CorredorI Patio IO-UAC Calcada-UAC Calcada-UED SaidaP1 DindinGourmet SaidaCarro1 SaidaP2 PistaCooper Avenida BrT - lugar
		d1 d2 d3 d4 d5 d6 d7 d8 d9 - dindin)
	(:init
		(Ligado S7 CorredorS)
		(Ligado CorredorS Escada)
		(Ligado Escada CorredorI)
		(Ligado CorredorI Patio)
		(Ligado Patio IO-UAC)
		(Ligado IO-UAC Calcada-UAC)
		(Ligado Calcada-UAC Calcada-UED)
		(Ligado Calcada-UED DindinGourmet)
		(Ligado Calcada-UED SaidaP1)
		(Ligado Calcada-UED SaidaP2)
		(Ligado Calcada-UED SaidaCarro1)
		(Ligado SaidaP1 PistaCooper)
		(Ligado SaidaP2 PistaCooper)
		(Ligado SaidaCarro1 PistaCooper)
		(Ligado PistaCooper Avenida)
		(Ligado Avenida BrT)
		(estou-em S7)
		(lugar-eh S7 S7)
		(passei-em S7)
		(comprou-dindin0)
		(tem-dindin d1)
		(tem-dindin d2)
		(tem-dindin d3)
		(tem-dindin d4)
		(tem-dindin d5)
		(tem-dindin d6)
		(tem-dindin d7)
		(tem-dindin d8)
		(tem-dindin d9)) 
	(:goal (estou-em BrT)))
