(define (domain caminhando)
 (:types lugar)
 (:predicates
   (Ligado ?l1 ?l2 - lugar)
   (estou-em ?l - lugar)
   (passei-em ?l - lugar)
   (comprou-dindin0)
   (comprou-dindin1)
   (comprou-dindin2)
   (comprou-dindin3)
 )

 (:action mover
   :parameters (?l1 ?l2 - lugar)
   :precondition (and (estou-em ?l1)
                       (or (Ligado ?l1 ?l2)
                           (Ligado ?l2 ?l1)))
   :effect (and (not (estou-em ?l1))
                 (passei-em ?l2)
                 (estou-em ?l2))
 )

 (:action comprardindin1
    :parameters (?l1 - lugar)
    :precondition (and (comprou-dindin0) (estou-em ?l1)
                       (= ?l1 DindinGourmet))
    :effect (and (comprou-dindin1) (not (comprou-dindin0)) ))

 (:action comprardindin2
    :parameters (?l1 - lugar)
    :precondition (and (comprou-dindin1) (estou-em ?l1)
                       (= ?l1 DindinGourmet))
    :effect (and (comprou-dindin2) (not (comprou-dindin1)) ))

 (:action comprardindin3
    :parameters (?l1 - lugar)
    :precondition (and (comprou-dindin2) (estou-em ?l1)
                       (= ?l1 DindinGourmet))
    :effect (and (comprou-dindin3) (not (comprou-dindin2)) ))

)
