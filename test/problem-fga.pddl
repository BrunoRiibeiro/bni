(define (problem indoparaobrt)
 (:domain caminhando)
 (:objects
	S7 CorredorS Escada CorredorI Patio IO-UAC Calcada-UAC Calcada-UED SaidaP1 DindinGourmet SaidaCarro1 SaidaP2 PistaCooper Avenida BrT - lugar
 )

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
   (passei-em S7)
   (comprou-dindin0)
 ) 

 (:goal
   (and
      (comprou-dindin2)
      (estou-em BrT)
   )
 )

)
