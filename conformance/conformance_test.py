from Levenshtein import distance as levenshtein_distance

from conformance.wasserstein import emd


def test_levenshtein():
    s1, s2 = "ab", "ba"
    assert levenshtein_distance(s1, s1) == 0
    assert levenshtein_distance(s1, s2) == 2


def test_wasserstein():
    l1 = [("ab", 0, 0, 0.3), ("ba", 0, 0, 0.7)]
    l2 = [("ab", 0, 0, 0.6), ("ba", 0, 0, 0.4)]
    assert emd(l1, l1) == 0
    assert emd(l1, l2) == 0.6
