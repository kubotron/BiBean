Experiment with the "Bibip" vario sound on [SkyBean](https://github.com/fhorinek/SkyBean)

Installation
============
Download the Bibip [firmware](https://raw.githubusercontent.com/kubotron/BiBean/master/bibip_0.1.3.ebin) and the [config](https://raw.githubusercontent.com/kubotron/BiBean/master/config-black-keys.sbc) that goes with it. Install the firmware and the config with the [SkyBean configurator chrome app](https://chrome.google.com/webstore/detail/skybean-configurator/njolekdacakglgbnpmeldongebgldnhd?hl=en). 

What is Bibip?
==============
Bibip is a pleasant and meaningful vario sound. Bibip makes **two beeps**, not just one.

When you're flying a paraglider: 
* either you are **climbing** in rising air
* or you are in **sinking** air, loosing height faster than usual
* or you are somewhere in between, in air that is **buoyant** but not really climbing. 

Bibip has a distinct sound sequence in each case:
* When climbing (above your configured climb threshold), it is rising: "boo-bip" 
* In sinking air (under the configured sink threshold), the melody is falling: "bip-boo"
* If buoyant (in between your lift and sink threshold), Bibip makes a flat "bip-bip"

Here is a [demo on youtube](https://www.youtube.com/watch?v=7PRNan8xcpo) 

Bibip is never quiet. The frequencies are based on black piano keys. Integration interval is 0.7sec. Play with the config, fly safe, stay tuned. I'm moving on to the SkyDrop platform for next steps with Bibip now. 
Kubo (bibip@kubokubo.net) 

Troubleshooting
===============
*To remove Bibip from your SkyBean* use the chrome configurator app to install the "skybean_beta2.ebin". If you saved your old configuration to a file, you can now load it to SkyBean, too.

*The links to firmware and config above lead to some weird text.* Use "Save link as..." option to save the firmware and config files. 

*To load bibip configuration* launch the SkyBean configurator app, Click on "Load config from file" and select the config file you downloaded. Note that you can change the config with the configurator app and then "Save config to SkyBean".   

*I edited the config file in a text editor and now the sounds are all messed up.* Don't use values greater than 2100 as tone length and pause duration, the device can't handle it. (They lead to integer overflow.)

*To install the bibip firmware* select "Update Skybean Firmware", in next dialog select "Load firmware from file" link at the bottom. Pick the bibip_0.1.3.ebin firmware file you downloaded. (Don't change the file in any way)

*I have trouble installing the firmware or the config with the chrome configurator app.* It takes multiple tries sometimes. I had the best luck when selecting the COM port manually (not plugging the skybean in and out). Open the app, and open new chrome tab afterwards. Type "chrome://inspect" into the adress bar. Select "Apps" from the menu left. Select the Skybean app, select "console". You will see events logged to the console. In the console, you will see if there are any errors during the firmware update.

*I still can't install the config on SkyBean, no matter what.* In [this firmware](https://raw.githubusercontent.com/kubotron/BiBean/master/bibip_0.1.3.hardcoded.ebin), Bibip sounds are hardcoded, you don't need any config

*Bibip has some shortcomings.* The duration of the gap in between the two beeps is not configurable. Bibip keeps beeping all the time.

Please, do open issues if you find bugs or are missing something. 


