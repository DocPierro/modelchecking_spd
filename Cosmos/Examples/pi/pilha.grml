<?xml version="1.0" encoding="UTF-8"?>

<model formalismUrl="http://formalisms.cosyverif.org/HASL-formula.fml" xmlns="http://cosyverif.org/ns/model">
  <attribute name="declaration">
    <attribute name="constants">
      <attribute name="intConsts">
      </attribute>
      <attribute name="realConsts">
      </attribute>
    </attribute>
    <attribute name="variables">
      <attribute name="reals">
        <attribute name="real">
          <attribute name="name">
            OK
          </attribute>
        </attribute>
        <attribute name="real">
          <attribute name="name">
            r
          </attribute>
        </attribute>
        <attribute name="real">
          <attribute name="name">
            y
          </attribute>
        </attribute>
        <attribute name="real">
          <attribute name="name">
            x
          </attribute>
        </attribute>
      </attribute>
      <attribute name="discretes">
      </attribute>
    </attribute>
  </attribute>
  <attribute name="HASLFormula">
    <attribute name="AVG">
      <attribute name="YHF">
        <attribute name="last">
          <attribute name="expr"><attribute name="name">
            OK
          </attribute></attribute>
        </attribute>
      </attribute>
    </attribute>
  </attribute>
  <node id="3" nodeType="state">
    <attribute name="invariant">
      <attribute name="boolExpr"><attribute name="boolValue">
        true
      </attribute></attribute>
    </attribute>
    <attribute name="name">
      l0
    </attribute>
    <attribute name="flows">
      <attribute name="flow">
        <attribute name="name">
          x
        </attribute>
        <attribute name="expr"><attribute name="numValue">
          1
        </attribute></attribute>
      </attribute>
    </attribute>
    <attribute name="type">
      Initial
    </attribute>
  </node>
  <node id="4" nodeType="state">
    <attribute name="invariant">
      <attribute name="boolExpr"><attribute name="boolValue">
        true
      </attribute></attribute>
    </attribute>
    <attribute name="name">
      l1
    </attribute>
    <attribute name="flows">
      <attribute name="flow">
        <attribute name="name">
          y
        </attribute>
        <attribute name="expr"><attribute name="numValue">
          1
        </attribute></attribute>
      </attribute>
    </attribute>
    <attribute name="type">
      Normal
    </attribute>
  </node>
  <node id="5" nodeType="state">
    <attribute name="invariant">
      <attribute name="boolExpr"><attribute name="boolValue">
        true
      </attribute></attribute>
    </attribute>
    <attribute name="name">
      lp
    </attribute>
    <attribute name="flows">
    </attribute>
    <attribute name="type">
      Final
    </attribute>
  </node>
  <node id="6" nodeType="state">
    <attribute name="invariant">
      <attribute name="boolExpr"><attribute name="boolValue">
        true
      </attribute></attribute>
    </attribute>
    <attribute name="name">
      lm
    </attribute>
    <attribute name="flows">
    </attribute>
    <attribute name="type">
      Final
    </attribute>
  </node>
  <node id="7" nodeType="state">
    <attribute name="invariant">
      <attribute name="boolExpr"><attribute name="boolValue">
        true
      </attribute></attribute>
    </attribute>
    <attribute name="name">
      l2
    </attribute>
    <attribute name="flows">
    </attribute>
    <attribute name="type">
      Normal
    </attribute>
  </node>
  <arc id="8" arcType="transition" source="3" target="4">
    <attribute name="updates">
    </attribute>
    <attribute name="guard">
      <attribute name="boolExpr"><attribute name="boolValue">
        true
      </attribute></attribute>
    </attribute>
    <attribute name="label">
      ALL
    </attribute>
  </arc>
  <arc id="9" arcType="transition" source="4" target="7">
    <attribute name="updates">
      <attribute name="update">
        <attribute name="name">
          r
        </attribute>
        <attribute name="expr"><attribute name="function"><attribute name="+">
          <attribute name="expr"><attribute name="function"><attribute name="*">
            <attribute name="expr"><attribute name="name">
              x
            </attribute></attribute>
            <attribute name="expr"><attribute name="name">
              x
            </attribute></attribute>
          </attribute></attribute></attribute>
          <attribute name="expr"><attribute name="function"><attribute name="*">
            <attribute name="expr"><attribute name="name">
              y
            </attribute></attribute>
            <attribute name="expr"><attribute name="name">
              y
            </attribute></attribute>
          </attribute></attribute></attribute>
        </attribute></attribute></attribute>
      </attribute>
    </attribute>
    <attribute name="guard">
      <attribute name="boolExpr"><attribute name="boolValue">
        true
      </attribute></attribute>
    </attribute>
    <attribute name="label">
      ALL
    </attribute>
  </arc>
  <arc id="10" arcType="transition" source="7" target="5">
    <attribute name="updates">
      <attribute name="update">
        <attribute name="name">
          OK
        </attribute>
        <attribute name="expr"><attribute name="numValue">
          4
        </attribute></attribute>
      </attribute>
    </attribute>
    <attribute name="guard">
      <attribute name="boolExpr"><attribute name="lessEqual">
        <attribute name="expr"><attribute name="name">
          r
        </attribute></attribute>
        <attribute name="expr"><attribute name="numValue">
          1
        </attribute></attribute>
      </attribute></attribute>
    </attribute>
    <attribute name="label">
      ALL
    </attribute>
  </arc>
  <arc id="11" arcType="transition" source="7" target="6">
    <attribute name="updates">
      <attribute name="update">
        <attribute name="name">
          OK
        </attribute>
        <attribute name="expr"><attribute name="numValue">
          0
        </attribute></attribute>
      </attribute>
    </attribute>
    <attribute name="guard">
      <attribute name="boolExpr"><attribute name="greaterEqual">
        <attribute name="expr"><attribute name="name">
          r
        </attribute></attribute>
        <attribute name="expr"><attribute name="numValue">
          1
        </attribute></attribute>
      </attribute></attribute>
    </attribute>
    <attribute name="label">
      ALL
    </attribute>
  </arc>
</model>