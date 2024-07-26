(define (domain meucafebistro)
  (:requirements :strips :typing :negative-preconditions)

  (:types capsula caneca cafeteira agua nivel - objeto)

  (:predicates 
               (capsula-usada ?cap - capsula)
               (segurando ?o - objeto)
               (tem-capsula ?c - cafeteira ?cap - capsula)
               (capsula-bloqueada ?cap - capsula)
               (alguma-capsula ?c - cafeteira)
               (caneca-em ?can - caneca ?c - cafeteira)
               (tem-caneca ?c - cafeteira)
               (caneca-bloqueada ?can - caneca)
               (cafe-pronto ?can - caneca ?cap - capsula))
  (:functions (nivel-atual ?c - cafeteira)
              (capsulas-cafe ?cap - capsula))
  ;;(:action COLOCARAGUA
  ;;  :parameters (?c - cafeteira)
  ;;  :precondition (not (tem-agua ?c))
  ;;  :effect (tem-agua ?c))
  (:action TIRARCAPSULA
    :parameters (?c - cafeteira ?cap - capsula)
    :precondition (tem-capsula ?c ?cap)
    :effect (and
              (not (capsula-bloqueada ?cap))
              (not (tem-capsula ?c ?cap))
              (not (alguma-capsula ?c))))
  (:action COLOCARCAPSULA
    :parameters (?c - cafeteira ?cap - capsula)
    :precondition (and
                   (not (capsula-bloqueada ?cap))
                   (not (alguma-capsula ?c))
                   (not (capsula-usada ?cap)))
    :effect (and (alguma-capsula ?c)
                 (capsula-bloqueada ?cap)
                 (tem-capsula ?c ?cap)))

  (:action COLOCARCANECA
    :parameters (?c - cafeteira ?can - caneca)
    :precondition (and
                  (not (tem-caneca ?c))
                  (not (caneca-bloqueada ?can)))
    :effect (and
              (caneca-em ?can ?c)
              (caneca-bloqueada ?can)
              (tem-caneca ?c)))
  (:action TIRARCANECA
    :parameters (?c - cafeteira ?can - caneca)
    :precondition (caneca-em ?can ?c)
    :effect (and
              (not (caneca-em ?can ?c))
              (not (tem-caneca ?c))
              (not (caneca-bloqueada ?can))))
  (:action FAZERCAFE
    :parameters (?c - cafeteira ?cap - capsula ?can - caneca)
    :precondition (and
                    (>= (nivel-atual ?c) 50)
                    (tem-capsula ?c ?cap)
                    (caneca-em ?can ?c)
                    (tem-caneca ?c))
    :effect (and
              (decrease (nivel-atual ?c) 50)
              (not (tem-capsula ?c ?cap))
              (not (alguma-capsula ?c))
              (not (capsula-bloqueada ?cap))
              (capsula-usada ?cap)
              (cafe-pronto ?can ?cap)))

)
