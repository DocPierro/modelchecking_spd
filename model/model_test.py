from model.Petrinet import Place, Transition, InArc, OutArc, Petrinet


def test_place():
    p1 = Place("p1", 1)
    assert p1.get_name() == "p1"
    assert p1.get_tokens() == 1
    assert p1.__str__() == "(p1,1)"


def test_transition():
    t1 = Transition("t1", 0.5)
    assert t1.get_name() == "t1"
    assert t1.get_weight() == 0.5
    assert t1.__str__() == "(t1,0.5)"


def test_arc():
    p1, t1 = Place("p1", 4), Transition("t1", 0.6)
    p2, t2 = Place("p2", 6), Transition("t2", 0.4)
    i1, o1 = InArc(p1, t1, 2), OutArc(t2, p2, 4)
    assert i1.get_source().get_name() == "p1"
    assert o1.get_target().get_tokens() == 6
    assert o1.get_multiplicity() == 4
    assert o1.__str__() == "(t2,p2,4)"


def test_petrinet():
    start, p1, p2, end = (Place("start", 1), Place("p1", 0),
                          Place("p2", 0), Place("end", 0))
    a, b, c, d, e = (Transition("a", 1), Transition("b", 0.6),
                     Transition("c", 0.4), Transition("d", 0.7),
                     Transition("e", 0.3))
    i1, i2, i3, i4, i5 = (InArc(start, a, 1),  InArc(p1, b, 1),
                          InArc(p1, c, 1), InArc(p2, d, 1),
                          InArc(p2, e, 1))
    o1, o2, o3, o4, o5 = (OutArc(a, p1, 1),  OutArc(b, p2, 1),
                          OutArc(c, p2, 1), OutArc(d, end, 1),
                          OutArc(e, end, 1))
    pn1 = Petrinet([start, p1, p2, end], [a, b, c, d, e],
                   [i1, i2, i3, i4, i5], [o1, o2, o3, o4, o5])
    assert pn1.get_places()[0] == start
    assert pn1.get_transitions()[0] == a
    assert pn1.get_inarcs()[0] == i1
    assert pn1.get_outarcs()[0] == o1
    assert (pn1.__str__() == "Places: [(start,1),(p1,0),(p2,0),(end,0)]\n" +
            "Transitions: [(a,1),(b,0.6),(c,0.4),(d,0.7),(e,0.3)]\n" +
            "Inarcs: [(start,a,1),(p1,b,1),(p1,c,1),(p2,d,1),(p2,e,1)]\n" +
            "Outarcs: [(a,p1,1),(b,p2,1),(c,p2,1),(d,end,1),(e,end,1)]")


def test_petrinet_pm4py():
    pn2 = Petrinet("../data/ex1.xes")
    assert sorted([place.name for place in pn2.get_places()]) == sorted(["source", "p_3", "p_4", "sink"])
    assert sorted([transition.name for transition in pn2.get_transitions()]) == sorted(["a", "b", "c", "d", "e"])
    assert sorted([ia.source.name for ia in pn2.get_inarcs()]) == sorted(["source", "p_3", "p_3", "p_4", "p_4"])
    assert sorted([oa.target.name for oa in pn2.get_outarcs()]) == sorted(["p_3", "p_4", "p_4", "sink", "sink"])


def test_mfc():
    pn = Petrinet("../data/ex1.xes")
    assert pn.get_enabled_transitions([pn.source]) == [transition for transition in pn.get_transitions() if transition.get_name() == "a"]
    assert pn.get_enabled_transitions([pn.sink]) == []
    assert pn.update([pn.source], [transition for transition in pn.get_transitions() if transition.get_name() == "a"][0]) == [place for place in pn.get_places() if place.get_name() == "p_3"]
    print("\n")
    print(pn.markings)
    print(pn.firing_sequences)
    print(pn.conflicts)


def test_eqs():
    pn = Petrinet("../data/ex1.xes")
    print("\n")
    print(pn.eqs)
