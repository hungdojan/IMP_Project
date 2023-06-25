# Zadání

### Předmět
[IMP - Mikroprocesorové a vestavěné systémy](?gm=gm_detail_predmetu&apid=230998)

Ak. rok: 2022/2023

Název: Š - ARM-FITkit3 či jiný HW: Měření vzdálenosti ultrazvukovým senzorem

Vedoucí: [Ing. Václav Šimek](/lide/vaclav-simek-17532)

### Popis

**Vedoucí:** Václav Šimek (simekv@fit.vutbr.cz)  
  
1\. Od vedoucího si **zapůjčte** vývojovou desku **FITkit3**, ultrazvukový měřič vzdálenosti HY-SRF05 a modul se segmentovým LED displejem.  
  
2\. V **Kinetis Design Studiu** nebo **MCUXpresso IDE** implementujte **vestavnou aplikaci** v jazyce C pro **Kinetis K60**, která bude měřit vzdálenost nejbližšího objektu od senzoru a její hodnotu (v centimetrech) bude zobrazovat na displeji.  
  
**Zájemci/kyně o řešení projektu na jiném technickém vybavení** (např. FITkit 1.2/1.3, Arduino, ESP32, Raspberry Pi, Zynq; toto vybavení je nutno opatřit si bez pomoci vedoucího, např. v knihovně FIT) zkonzultují toto vybavení s vedoucím a řešit začnou až s jeho souhlasem.  
Nemáte-li odsouhlaseno jiné technické vybavení, předpokládá se řešení na platformě **FITkit 3**, kterou si za tímto účelem včas zapůjčíte u vedoucího projektu.  
  
Z volně dostupného SW je povoleno použít SDK pro MCU K60 (dostupný z [http://mcuxpresso.nxp.com/](http://mcuxpresso.nxp.com/)).

\*\*\*\*\*\*\*\*\*\*\*

Dílčí složkou pro hodnocení je též stručná **dokumentace** poskytující základní informace o projektu a způsobu řešení. Ta by měla obsahovat zejména:

*   Odstavec s textovým popisem projektu - o co se jedná, účel, funkce.
*   Popis HW zapojení použitých komponent.
*   Popis způsobu řešení - stačí jen info o klíčových částech implementovaných řešitelem.
*   Závěrečné shrnutí - zhodnocení řešení, známé problémy a nedostatky, sdělení o případných nedokončených/neimplementovaných částech s ohledem na zadání.

Není podstatný ani tak rozsah dokumentace, ale to, zda obsahuje **podstatné informace** o řešení projektu.  
  
**Odevzdávejte** jediný ZIP archiv pojmenovaný Vaším loginem, který bude obsahovat adresář opět nazvaný podle loginu a v něm řešení projektu (**projekt**, bez binárních souborů sestavitelných pomocí KDS/MCUXpresso IDE na základě zdrojových souborů v projektu, a **dokumentaci** ve formátu PDF).  
  
Jednou z podmínek udělení plného hodnocení je osobní demonstrace funkčnosti řešení vedoucímu projektu. Hromadné obhajoby proběhnou po termínu odevzdání. Studenti, kteří budou mít project dokončený a odevzdaný předčasně, budou moci využít jedné hromadné obhajoby před termínem odevzdání. Informace k obhajobám budou včas sděleny mailem.  
  
Zapůjčení vybavení bude možné od konce října. Informace o zapůjčení budou rovněž sděleny mailem.

**Podpůrné materiály k řešení projektu:**

1.  [Schéma zapojení desky FITkit v3](https://www.fit.vutbr.cz/~simekv/IMP_projekt_board_FITkit_v3.0_schematic.pdf)
2.  [Doprovodné informace k zadání](https://www.fit.vutbr.cz/~simekv/IMP_projekt_mereni_vzdalenost_ultrazvuk.pdf)
3.  [Vzorový projekt k otestování vybavení](https://www.fit.vutbr.cz/~simekv/IMP_projekt_ultrazvuk_tep_LED_disp_test.zip)
4.  [Inslatační balíček prostředí KDS v3.0.0](https://www.fit.vutbr.cz/~simekv/KDS_install_v3.0.0.exe)

