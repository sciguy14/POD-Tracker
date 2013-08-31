Real-Time POD Tracker
=====================
Learn more about this project on my website: http://www.jeremyblum.com/2013/08/31/pod-tracker  
Disclaimer: As I explain on my website, I am making no guarantees about the operability of this system. It is provided as is. The system does work, but in its current state is not reliable over extended periods of time with different wireless and GPS conditions. Please feel free to fork this project and improve upon it. If I have a need to do long distance real-time tracking again in the future, I will improve this software to make it more robust.

Instructions for Use
--------------------
* Set up the system following the diagram and notes from my website.
* Set up a Twilio Account and get a trial number and an SID. (http://www.twilio.com)
* Set up Google Maps API v3 access and get your API Key (https://developers.google.com/maps/documentation/javascript/tutorial#api_key)
* Rename the web/config_sample.php file to web/config.php. Fill in the appopriate values. Upload it to your server.
* In your Twilio account configuration panel, set up your SMS number Messaging Request URL to point to the request.php file that you just uploaded (ex. http://www.example.com/pod/request.php), and set the request type to POST.
* Install the Arduino Narcoleptic Library (https://code.google.com/p/narcoleptic/)
* Open the send_text.ino file and insert your Twilio SMS number on line 77, in place of the 10 X's. Do not use dashes.
* Upload onto your Arduino, ensuring that the shield is removed.
* Reinstall the shield and let it roll!

License
-------
This work is licensed under a Creative Commons Attribution-ShareAlike 3.0 Unported License.  
Please share improvements or remixes with the community, and attribute me (Jeremy Blum, http://www.jeremyblum.com) when reusing portions of my code.
http://creativecommons.org/licenses/by-sa/3.0/deed.en_US
