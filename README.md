My experiment with the "Bibip" vario sound on SkyBean  
(the official SkyBean repo is https://github.com/fhorinek/SkyBean)

What is Bibip?
==============
Bibip is a relatively pleasant and meaningful vario sound. 

Bibip makes **two beeps**, not just one.

When you're flying a paraglider: 
* either you are **climbing** more than 0.5m/s
* or you are in **sinking** air, sinking faster than -1.5m/s
* or you are somewhere in between -1.5 and 0.5 m/s. Not really climbing, but somewhat  **buoyant**.

Bibip makes a distinct sound sequence in each case:
* If you're climbing, bibip makes a rising seqence: "boo-bip"
* If you're in sinking air, bibip sequence is falling: "bip-boo"
* If you're buoyant, bibip makes a flat sequence: "bip-bip"

The frequencies are based on black piano keys. Integration interval is 0.7sec. 


Installation
============
Download the bibip [firmware](https://raw.githubusercontent.com/kubotron/SkyBiBean/master/bibip_0.1.ebin) and the [config](https://raw.githubusercontent.com/kubotron/SkyBiBean/master/config-black-keys.sbc) that goes with it. Install the firmware and the config with the official [SkyBean configurator chrome app](https://chrome.google.com/webstore/detail/skybean-configurator/njolekdacakglgbnpmeldongebgldnhd?hl=en). ("Load config from file" and then "Save config to Skybean", then select "Update Skybean Firmware", then "Load firmware from file")


Troubleshooting
===============
*The links to firmware and config above lead to some weird text.*
Use "Save link as..." option to save the firmware and config files. 

*I have trouble installing config on SkyBean.* 
Bibip sounds are hardcoded in [this firmware](https://raw.githubusercontent.com/kubotron/SkyBiBean/master/bibip_0.1.hardcoded.ebin), you don't need config

*Bibip has some shortcomings.* 
Bibip is never quiet. It doesn't respect the configured sink and lift threshold. The duration of the gap in between the two beeps is not configurable. 


Questions?
==========
Suggestions? Do you know anything about sounds and music theory? Do you hate vario sounds too? 
bibip@kubokubo.net 

Please, do open issues if you find bugs or are missing something or want to comment on some specific bibip feature. I'm moving on to the SkyDrop platform for next steps with Bibip.

Have fun, play with the configs, fly safe, tell me what you think. 
Kubo

