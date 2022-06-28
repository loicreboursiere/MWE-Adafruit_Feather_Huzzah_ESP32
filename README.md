# MWE-Adafruit_Feather_Huzzah_ESP32

Connection d'un module [https://learn.adafruit.com/adafruit-ultimate-gps](Adafruit Ultimate GPS) à la carte [https://learn.adafruit.com/adafruit-huzzah32-esp32-feather](Adafruit Feather ESP32 WIFI+BLE)

<!-- TABLE DES MATIÈRES -->
<details open="open">
  <summary><h2 style="display: inline-block">Table des matières</h2></summary>
  <ol>
    <li><a href="#installation">Installation</a></li>
      <ul>
        <li><a href="#esp32-et-Arduino-IDE">ESP32 et Arduino IDE</a></li>
        <li><a href="#Adafruit-Feather-ESP32-et-module-GPS">Adafruit Feather ESP32 et module GPS</a></li>
      </ul>
    <li><a href="#contribution">Contribution</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#remerciements">Remerciements</a></li>
  </ol>
</details>



## Installation
### ESP32 et Arduino IDE
Les cartes à base d'ESP32, comme d'autres cartes non développées par Arduino.cc,  ne sont pas par défaut dans l'IDE Arduino. 
La méthode pour les ajouter (identique pour la plupart des autres cartes non-Arduino mais pouvant être programmées à l'aide de l'IDE Arduino) est la suivante :
- ouvrir l'IDE Arduino
- aller dans `Preferences>Additional boards manager URLs`
- ajouter l'URL suivante : `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
- s'il y a plusieurs URL celles-ci sont séparées par une `,`. Il est aussi possible de cliquer sur l'icône situé au bout du champ de texte et d'ajouter la nouvelle URL sur une nouvelle ligne. L'IDe ajoutera lui-même la virgule entre les deux URLs.

La deuxième étape est d'installer les drivers [https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers](VCP CP210x) pour ESP32 (pour d'autres ESP la manipulation sera la même, mais les drivers peuvent changer).
Sous Linux, une fois le dossier téléchargé et décompressé, les commandes suivantes (extraites du fichier  sont à réaliser : 
- `make`
- `cp cp210x.ko to /lib/modules/<kernel-version>/kernel/drivers/usb/serial`
- `insmod /lib/modules/<kernel-version/kernel/drivers/usb/serial/usbserial.ko`
- `insmod cp210x.ko`
- `reboot`

Dans Arduino IDE sélectionner le port correspondant à la carte et choisir la carte `Adafruit ESP32 Feather`. 

Tester le code de l'exemple `Blink`. Si une erreur d'upload `No module named serial` apparaît, il semble qu'une installation du package python `pyserial`règle le problème : 
`python -m pip install pyserial`

### Adafruit Feather ESP32 et module GPS

Dans l'IDE Arduino, installer la bibliothèque Adafruit GPS : `Library Manager > Adafruit GPS` ([https://github.com/adafruit/Adafruit_GPS](Github)).





## Utilisation du dépôt

### Cloner

```

## Contribution

## License

## Contact

## Remerciements
