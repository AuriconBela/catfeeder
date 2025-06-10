# Catfeeder

Ez a projekt egy Arduino-alapú automata macskaetető vezérlő. A rendszer főbb jellemzői:

- **Automatikus etetés**: Előre beállított időpontokban adagolja az ételt egy szervomotor segítségével.
- **Valós idejű óra (RTC)**: Pontos időzítés a DS3231 RTC modullal.
- **LCD kijelző**: Az aktuális állapot, idő és beállítások megjelenítése.
- **Három gomb**: Időzítés és egyéb funkciók beállításához.
- **GY-9930 közelítés szenzor**: Az LCD kijelző csak akkor aktív, ha valaki 30 cm-en belül van.
- **Állapotgép (State pattern)**: A működés különböző állapotokban zajlik (pl. normal, proximity, rollup, open, rolldown, hour set, minute set), szigorúan meghatározott átmenetekkel.

## Főbb állapotok
- Normal
- Proximity
- Rollup
- Open
- Rolldown
- Hour set
- Minute set

## Átmenetek
Az állapotok között csak előre meghatározott módon lehet váltani, a proximity szenzor és a gombok hatására.

## Hardver követelmények
- Arduino Micro vagy kompatibilis
- DS3231 RTC modul
- LiquidCrystal LCD (16x2)
- 3 db nyomógomb
- GY-9930 proximity szenzor
- Szervomotor

## Szoftver
A projekt PlatformIO-val fordítható és tölthető fel az eszközre.

---
