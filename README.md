This is my experiment with the "Bibip" vario sound on SkyBean  
(the official SkyBean repo is https://github.com/fhorinek/SkyBean)

What is Bibip?
==============
Bibip is a first step on my search for more pleasant and meaningful vario sounds.

Bibip makes **two beeps**, not just one.

It has three modes: 
* either you are **climbing** more than 0.5m/s
* or you are in **sinking** air, sinking more than -1.5m/s
* or you are somewhere in between -1.5 and 0.5 m/s. 
You're not really climbing, but the air is **buoyant**.

* If you're climbing, bibip makes a rising seqence: "boo-bip"
* If you're in sinking air, bibip sequence is falling: "bip-boo"
* If you're buoyant, bibip makes a flat sequence: "bip-bip"

The frequencies in the config are based on black piano keys. 

I also chose a longer integration interval in the config: 0.7sec.

Bibip is never quiet at the moment (It doesn't respect the configured sink and lift threshold: I can change this if there is need)


Installation
============
Download the bibip [firmware](https://raw.githubusercontent.com/kubotron/SkyBiBean/master/bibip_0.1.ebin) and the [config](https://raw.githubusercontent.com/kubotron/SkyBiBean/master/config-black-keys.sbc) that goes with it. Install the firmware and the config with the official [SkyBean configurator chrome app](https://chrome.google.com/webstore/detail/skybean-configurator/njolekdacakglgbnpmeldongebgldnhd?hl=en).

If you have trouble installing config on your SkyBean (like I do), you can grab a firmware where bibip sounds are hardcoded: 
[bibip_0.1.hardcoded.ebin](https://raw.githubusercontent.com/kubotron/SkyBiBean/master/bibip_0.1.hardcoded.ebin)


Questions?
==========
Do you have questions or suggestions? Do you know anything about sounds and music theory? Do you hate vario sounds too? Do you hate Bibip? 
Feel free to hit me up on bibip@kubokubo.net 

Please, do open issues if you find bugs or are missing something. That being said, I don't expect to work a whole lot more on the Bibip for SkyBean.

I'm moving on to the SkyDrop platform now, which is a bit more promising for what I want to try next:

* more resolution around zero, perhaps at the expense of resolution at the extremes around -10m/s and +10m/s
* applying a simple envelope to the sounds for more natural feeling
* experimenting with harmonic frequencies to add some character to the sound

Fly safe, have fun, tell me what you think. 
Kubo

