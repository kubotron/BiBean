Experiment with the "Bibip" vario sound on [SkyBean](https://github.com/fhorinek/SkyBean)

Installation
============
Download the Bibip [firmware](https://raw.githubusercontent.com/kubotron/SkyBiBean/master/bibip_0.1.ebin) and the [config](https://raw.githubusercontent.com/kubotron/SkyBiBean/master/config-black-keys.sbc) that goes with it. Install the firmware and the config with the [SkyBean configurator chrome app](https://chrome.google.com/webstore/detail/skybean-configurator/njolekdacakglgbnpmeldongebgldnhd?hl=en). 

What is Bibip?
==============
Bibip is a pleasant and meaningful vario sound. Bibip makes **two beeps**, not just one.

When you're flying a paraglider: 
* either you are **climbing** more than 0.5m/s
* or you are in **sinking** air, loosing height faster than -1.5m/s
* or you are somewhere in between -1.5 and 0.5 m/s. Not really climbing, but somewhat  **buoyant**.

Bibip has a distinct sound sequence in each case:
* When climbing, it is rising: "boo-bip"
* In sinking air, the melody is falling: "bip-boo"
* If buoyant, Bibip makes a flat "bip-bip"

The frequencies are based on black piano keys. Integration interval is 0.7sec. Play with the config, fly safe, stay tuned.
I'm moving on to the SkyDrop platform for next steps with Bibip now :P Kubo (bibip@kubokubo.net) 

Troubleshooting
===============
*To remove Bibip from your SkyBean* use the chrome configurator app to install the "skybean_beta2.ebin". If you saved your old configuration to a file, you can now load it to SkyBean, too.

*The links to firmware and config above lead to some weird text.* Use "Save link as..." option to save the firmware and config files. 

*To load bibip configuration* launch the SkyBean configurator app, Click on "Load config from file" and select the config file you downloaded. Note that you can change it in a text editor or with the app. Then "Save config to SkyBean".   

*To install the bibip firmware* select "Update Skybean Firmware", in next dialog select "Load firmware from file" link at the bottom. Pick the bibip_0.1.ebin firmware file you downloaded. (Don't change the file in any way)

*I have trouble installing config on SkyBean.* In [this firmware](https://raw.githubusercontent.com/kubotron/SkyBiBean/master/bibip_0.1.hardcoded.ebin), Bibip sounds are hardcoded, you don't need any config

*Bibip has some shortcomings.* Bibip is never quiet. It doesn't respect the configured sink and lift threshold. The duration of the gap in between the two beeps is not configurable. 

Please, do open issues if you find bugs or are missing something. 


