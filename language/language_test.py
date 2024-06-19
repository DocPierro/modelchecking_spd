from language.Eventlog import Event, Trace, Eventlog


def test_event():
    a1 = Event("1", "a", "2012-12-30T00:00:00")
    assert a1.get_case() == "1"
    assert a1.get_activity() == "a"
    assert a1.get_timestamp() == "2012-12-30T00:00:00"
    assert a1.__str__() == "Case: 1; Activity: a; Timestamp: 2012-12-30T00:00:00;"


def test_trace():
    t1 = Trace("0",
               [Event("1", "a", "2012-12-30T00:00:00"),
                    Event("1", "b", "2012-12-30T00:00:01"),
                    Event("1", "d", "2012-12-30T00:00:02")])
    assert t1.get_id() == "0"
    assert t1.get_sequence() == "abd"
    assert t1.__str__() == "<a,b,d>"


def test_eventlog():
    t1 = Trace(0,
               [Event("1", "a", "2012-12-30T00:00:00"),
                    Event("1", "b", "2012-12-30T00:00:01"),
                    Event("1", "d", "2012-12-30T00:00:02")])
    t2 = Trace(1,
               [Event("2", "a", "2012-12-30T00:00:10"),
                    Event("2", "c", "2012-12-30T00:00:11"),
                    Event("2", "d", "2012-12-30T00:00:12")])
    t3 = Trace(2,
               [Event("2", "a", "2012-12-30T00:00:20"),
                    Event("2", "b", "2012-12-30T00:00:21"),
                    Event("2", "e", "2012-12-30T00:00:22")])
    t4 = Trace(3,
               [Event("3", "a", "2012-12-30T00:00:30"),
                    Event("3", "c", "2012-12-30T00:00:31"),
                    Event("3", "e", "2012-12-30T00:00:32")])
    ev1 = Eventlog([t1, t2, t3, t4], [42, 28, 18, 12])
    assert ev1.get_traces()[0].get_id() == 0
    assert ev1.get_freqs() == [42, 28, 18, 12]
    assert ev1.get_probs() == [0.42, 0.28, 0.18, 0.12]
    assert ev1.__str__() == "[<a,b,d>*(42|0.42),\n<a,c,d>*(28|0.28),\n<a,b,e>*(18|0.18),\n<a,c,e>*(12|0.12)]"


def test_eventlog_pm4py():
    ev2 = Eventlog("../data/ex1.xes")
    assert ev2.get_traces()[0].get_id() == 0
    assert ev2.get_freqs() == [42, 28, 18, 12]
    assert ev2.get_probs() == [0.42, 0.28, 0.18, 0.12]
    assert ev2.__str__() == "[<a,b,d>*(42|0.42),\n<a,c,d>*(28|0.28),\n<a,b,e>*(18|0.18),\n<a,c,e>*(12|0.12)]"


def test_lha():
    ev = Eventlog("../data/ex1.xes")
    assert ev.get_activities() == {"a": 1, "b": 2, "c": 3, "d": 4, "e": 5}
    assert ev.get_language() == [('abd', 111, 0, 0.42), ('acd', 116, 1, 0.28),
                                 ('abe', 136, 2, 0.18), ('ace', 141, 3, 0.12),
                                 ('', -1, 4, 0.0)]
