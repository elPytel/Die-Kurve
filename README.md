# Semestrální práce z předmětu APO na oboru Kybernetika a Robotika

- [Semestrální práce z předmětu APO na oboru Kybernetika a Robotika](#semestrální-práce-z-předmětu-apo-na-oboru-kybernetika-a-robotika)
  - [Achtung die Kurve!](#achtung-die-kurve)
  - [Instalce závislostí](#instalce-závislostí)
  - [Kompilace](#kompilace)
  - [Spuštění](#spuštění)
    - [Ovládání hry](#ovládání-hry)

## Achtung die Kurve!
Jde o implementaci hry Achtung die Kurve! v jazyce C pro školní vývojovou desku. Součástí implementace je emulace na PC, kde je možné hru ovládat klávesnicí.

## Instalce závislostí

Před spuštěním projektu je nutné nainstalovat všechny závislosti. To lze provést pomocí následujícího příkazu:

```bash
./install.sh
```

> [!note]
> Nebo ručně podle souboru `dependencies.txt`

## Kompilace

Pro kompilaci projektu použijte následující příkaz:

```bash
make
```

## Spuštění

Pro spuštění emulace na PC použijte následující příkaz:

```bash
make run
```

nebo:
```bash
./aposem-main
```

### Ovládání hry

Rotace knoflíků je emulovaná pomocí klávesnice:

Levý knoflík:
- D 
- F
  
Pravý knoflík:
- J
- K