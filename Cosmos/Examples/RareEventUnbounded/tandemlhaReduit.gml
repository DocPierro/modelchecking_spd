<?xml version="1.0" encoding="UTF-8"?>

<model formalismUrl="http://alligator.lip6.fr/lha.fml" xmlns="http://gml.lip6.fr/model">
  <attribute name="declarations">
    <attribute name="variable">
      <attribute name="varName">
        x
      </attribute>
    </attribute>
    <attribute name="intConstDeclaration">
      <attribute name="constName">
        N
      </attribute>
      <attribute name="intFormula">
        <attribute name="value">
          50
        </attribute>
      </attribute>
    </attribute>
  </attribute>
  <attribute name="HASLFormula">
    <attribute name="avg">
      <attribute name="last">
        <attribute name="variable">
          x
        </attribute>
      </attribute>
    </attribute>
  </attribute>
  <node id="3" nodeType="location">
    <attribute name="invariant">
      <attribute name="boolFormula">
        <attribute name="and">
          <attribute name="less">
            <attribute name="plus">
              <attribute name="marking">
                RE_Queue1
              </attribute>
              <attribute name="marking">
                RE_Queue2
              </attribute>
            </attribute>
            <attribute name="intConst">
              N
            </attribute>
          </attribute>
          <attribute name="greater">
            <attribute name="plus">
              <attribute name="marking">
                RE_Queue1
              </attribute>
              <attribute name="marking">
                RE_Queue2
              </attribute>
            </attribute>
            <attribute name="value">
              0
            </attribute>
          </attribute>
        </attribute>
      </attribute>
    </attribute>
    <attribute name="name">
      s0
    </attribute>
    <attribute name="flow">
    </attribute>
    <attribute name="type">
      Initial
    </attribute>
  </node>
  <node id="4" nodeType="location">
    <attribute name="invariant">
      <attribute name="boolFormula">
        <attribute name="equal">
          <attribute name="plus">
            <attribute name="marking">
              RE_Queue1
            </attribute>
            <attribute name="marking">
              RE_Queue2
            </attribute>
          </attribute>
          <attribute name="intConst">
            N
          </attribute>
        </attribute>
      </attribute>
    </attribute>
    <attribute name="name">
      sp
    </attribute>
    <attribute name="flow">
    </attribute>
    <attribute name="type">
      Final
    </attribute>
  </node>
  <node id="5" nodeType="location">
    <attribute name="invariant">
      <attribute name="boolFormula">
        <attribute name="equal">
          <attribute name="plus">
            <attribute name="marking">
              RE_Queue1
            </attribute>
            <attribute name="marking">
              RE_Queue2
            </attribute>
          </attribute>
          <attribute name="value">
            0
          </attribute>
        </attribute>
      </attribute>
    </attribute>
    <attribute name="name">
      sm
    </attribute>
    <attribute name="flow">
    </attribute>
    <attribute name="type">
      Normal
    </attribute>
  </node>
  <arc id="6" arcType="arc" source="3" target="4">
    <attribute name="update">
      <attribute name="updatevar">
        <attribute name="variable">
          x
        </attribute>
        <attribute name="realFormula">
          <attribute name="value">
            1
          </attribute>
        </attribute>
      </attribute>
    </attribute>
    <attribute name="guard">
      <attribute name="boolFormula">
        <attribute name="boolean">
          true
        </attribute>
      </attribute>
    </attribute>
    <attribute name="action">
      ALL
    </attribute>
  </arc>
  <arc id="7" arcType="arc" source="3" target="5">
    <attribute name="update">
      <attribute name="updatevar">
        <attribute name="variable">
          x
        </attribute>
        <attribute name="realFormula">
          <attribute name="value">
            0
          </attribute>
        </attribute>
      </attribute>
    </attribute>
    <attribute name="guard">
      <attribute name="boolFormula">
        <attribute name="boolean">
          true
        </attribute>
      </attribute>
    </attribute>
    <attribute name="action">
      ALL
    </attribute>
  </arc>
  <arc id="8" arcType="arc" source="3" target="3">
    <attribute name="update">
    </attribute>
    <attribute name="guard">
      <attribute name="boolFormula">
        <attribute name="boolean">
          true
        </attribute>
      </attribute>
    </attribute>
    <attribute name="action">
      ALL
    </attribute>
  </arc>
</model>
