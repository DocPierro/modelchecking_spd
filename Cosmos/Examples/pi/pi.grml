<?xml version="1.0" encoding="UTF-8"?>

<model formalismUrl="http://formalisms.cosyverif.org/sptgd-net.fml" xmlns="http://cosyverif.org/ns/model">
  <node id="3" nodeType="place">
    <attribute name="name">
      P1
    </attribute>
    <attribute name="marking">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
  </node>
  <node id="4" nodeType="place">
    <attribute name="name">
      P2
    </attribute>
    <attribute name="marking">
      <attribute name="expr"><attribute name="numValue">
        0
      </attribute></attribute>
    </attribute>
  </node>
  <node id="5" nodeType="place">
    <attribute name="name">
      P3
    </attribute>
    <attribute name="marking">
      <attribute name="expr"><attribute name="numValue">
        0
      </attribute></attribute>
    </attribute>
  </node>
  <node id="6" nodeType="transition">
    <attribute name="weight">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
    <attribute name="priority">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
    <attribute name="name">
      sampx
    </attribute>
    <attribute name="service">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
    <attribute name="distribution">
      <attribute name="type">
        UNIFORM
      </attribute>
      <attribute name="param">
        <attribute name="number">
          0
        </attribute>
        <attribute name="expr"><attribute name="numValue">
          0
        </attribute></attribute>
      </attribute>
      <attribute name="param">
        <attribute name="number">
          1
        </attribute>
        <attribute name="expr"><attribute name="numValue">
          1
        </attribute></attribute>
      </attribute>
    </attribute>
  </node>
  <node id="7" nodeType="transition">
    <attribute name="weight">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
    <attribute name="priority">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
    <attribute name="name">
      sampy
    </attribute>
    <attribute name="service">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
    <attribute name="distribution">
      <attribute name="type">
        UNIFORM
      </attribute>
      <attribute name="param">
        <attribute name="number">
          0
        </attribute>
        <attribute name="expr"><attribute name="numValue">
          0
        </attribute></attribute>
      </attribute>
      <attribute name="param">
        <attribute name="number">
          1
        </attribute>
        <attribute name="expr"><attribute name="numValue">
          1
        </attribute></attribute>
      </attribute>
    </attribute>
  </node>
  <node id="8" nodeType="transition">
    <attribute name="weight">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
    <attribute name="priority">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
    <attribute name="name">
      fin
    </attribute>
    <attribute name="service">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
    <attribute name="distribution">
      <attribute name="type">
        DETERMINISTIC
      </attribute>
      <attribute name="param">
        <attribute name="number">
          0
        </attribute>
        <attribute name="expr"><attribute name="numValue">
          0
        </attribute></attribute>
      </attribute>
    </attribute>
  </node>
  <arc id="9" arcType="arc" source="3" target="6">
    <attribute name="valuation">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
  </arc>
  <arc id="10" arcType="arc" source="6" target="4">
    <attribute name="valuation">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
  </arc>
  <arc id="11" arcType="arc" source="4" target="7">
    <attribute name="valuation">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
  </arc>
  <arc id="12" arcType="arc" source="7" target="5">
    <attribute name="valuation">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
  </arc>
  <arc id="13" arcType="arc" source="5" target="8">
    <attribute name="valuation">
      <attribute name="expr"><attribute name="numValue">
        1
      </attribute></attribute>
    </attribute>
  </arc>
</model>