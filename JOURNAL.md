# 2026-06-11: Transfered project!

**Total time spent: 1.2 hours**

<img width="1433" height="1286" alt="image" src="https://github.com/user-attachments/assets/f6a15d2b-a7e1-4f9f-9b52-b929deb7c9ae" />

Transfered project from macando to forge, it was an enourmous pita to copy and paste all the journals and format everything

# 2026-06-11: Got software I2C to work

**Total time spent: 4.2 hours**

![image.png](https://cdn.hackclub.com/019eb5d8-f470-7a05-b3c2-0414f5532a6a/image.png)

implemented custom software I2C to use the amient light sensor - it became a lot more accurate

You can see the dips n stuff clearly - it works! It can detect flashes

# 2026-06-11: Flash test! 

**Total time spent: 3.8 hours**

Today I dissasembled a flash to test the light sensing.

Fumbled with bare-metal and ch32fun to write software I2C for sensing chip.

![image.png](https://cdn.hackclub.com/019eb5d3-9ee9-7778-9a95-2a947e9f89ae/image.png)

Got this at first

![image.png](https://cdn.hackclub.com/019eb5d3-bdc7-74a4-a75f-a6961908be57/image.png)

Then I disassembled this flash

Reverse engineered its circuits, got it to flash and got a new graph!

Software I2C was a huge pita

![image.png](https://cdn.hackclub.com/019eb5d3-d546-7db2-a8ee-9c0d7be1e265/image.png)

# 2026-06-11: Wrote the firmware! 

**Total time spent: 6.3 hours**

![image.png](https://cdn.hackclub.com/019eb5d2-ad81-7ab4-a46e-c51a47cbc610/image.png)

After getting my programmer, I finally can write my firmware. 

![image.png](https://cdn.hackclub.com/019eb5d2-cf57-72e6-ac52-57bbbd251379/image.png)

The stuff was in bare metal, so a tiny tad hard

![image.png](https://cdn.hackclub.com/019eb5d2-e1e0-7b89-8734-55857606a8b4/image.png)

![image.png](https://cdn.hackclub.com/019eb5d2-f363-72e6-8f9c-31e5d4ed462c/image.png)

![image.png](https://cdn.hackclub.com/019eb5d3-0606-7576-a277-9d8b0c1259d7/image.png)

Had to read through pages of memaps and stuff - this isn't like arduinos and rpis where everything is abstract and have sdks, there isn't even any libs available 

![image.png](https://cdn.hackclub.com/019eb5d3-4e35-7666-a06a-1b208b62f3bc/image.png)

But I got this nice brightness graph from the photodiode!

# 2026-06-11: Finally I gave up and ordered an linke 

**Total time spent: 2.3 hours**

After some more research, I gave up and ordered a linke ... very big sob and very sad I know........

![image.png](https://cdn.hackclub.com/019eb5d1-e8cb-7b7a-81ed-3acfeeb6276d/image.png)

Discussed it a bit more with people on discord to try to figure everything out, to no avail

![image.png](https://cdn.hackclub.com/019eb5d2-6cd9-7816-872f-44c7ac450be1/image.png)

# 2026-06-11: Then I even tried to code my ownprogrammer 

**Total time spent: 6.4 hours**

![image.png](https://cdn.hackclub.com/019eb5d0-aa0d-7e27-b4c6-e55d960acd86/image.png)

I even tried to program my own programmer with tinyusb and a pico.

Learnt about USB with https://beyondlogic.org/usbnutshell/usb1.shtml

![image.png](https://cdn.hackclub.com/019eb5d0-c6ff-718a-b8c4-4e78c61d1369/image.png)

found this site

![image.png](https://cdn.hackclub.com/019eb5d0-f817-7190-8ccf-3ddde8243b00/image.png)

And even wrote my own USB descriptors and stuff

![image.png](https://cdn.hackclub.com/019eb5d1-1d76-7ead-98a9-c6c8a7f1c51b/image.png)

Endpoints are 1:1 imitiations to real linke, but then I couldn't reverse-engineer protocol due to not having a linke

![image.png](https://cdn.hackclub.com/019eb5d1-39d6-7cd9-85c8-8191fb8f533f/image.png)

It became aval to enum, but there was no public protocol documentation...

Code is on github repo

# 2026-06-11: Try to alt programmers 

**Total time spent: 4.3 hours**

Then I tried to use alt programmers.

Like https://gitlab.com/BlueSyncLine/arduino-ch32v003-swio and https://github.com/aappleby/picorvd, but both didn't work...

![image.png](https://cdn.hackclub.com/019eb5cf-ff6f-7716-a839-d48293a79aca/image.png)
![image.png](https://cdn.hackclub.com/019eb5d0-0f58-7cf6-9c95-5ac2f00e3f8f/image.png)
![image.png](https://cdn.hackclub.com/019eb5d0-353f-717c-a7d8-6ea0105d66f8/image.png)
![image.png](https://cdn.hackclub.com/019eb5d0-2865-7a1f-bc87-81148764888f/image.png)

Even made my own edits in picorvd to try and debug stuff, to no avail. GDB was always returning 0




# 2026-06-11: Soldered side-project to debug 

**Total time spent: 4.7 hours**

![image.png](https://cdn.hackclub.com/019eb5cf-8713-7b4b-81d8-354faca10a76/image.png)

Then I soldered one of these comus - also hand pnp and solder reflow, but all 0201s. It was hell and printed a plate too!

It was only 1cmx2cm so pretty hard to solder

![image.png](https://cdn.hackclub.com/019eb5cf-9daa-72d2-b9c2-e98e2eb7946a/image.png)

In the same time I also soldered wires on a old USB A project to try to program it easily, and yep, it didn't work!

# 2026-06-11: Broke programmer and soldered 3 more boards 

**Total time spent: 7.2 hours**

Welp guess what, I broke my programmer

Tried to debug why my programmer was returning 0000000000s

Was damn weird...... And to debug it, I unsoldered ~2 known-good boards from past projects, and assembled more boards:

![image.png](https://cdn.hackclub.com/019eb5cd-7de4-7a50-a595-7d32e847342a/image.png)

Spent a ton of time soldering 3 more PCBs. Hand assemble, hand solder.

![image.png](https://cdn.hackclub.com/019eb5ce-feb5-7e5d-bf26-724aa1ab94d8/image.png)

Used stencil to apply paste

# 2026-06-11: Soldered the PCB!

**Total time spent: 4.7 hours**

![image.png](https://cdn.hackclub.com/019eb5cc-19ad-72d3-b586-0671a79cb12d/image.png)

Good thing I had the interactive BOM to use with me

![image.png](https://cdn.hackclub.com/019eb5cc-b324-75d0-86b5-41a7d6e8f787/image.png)

So soldered everything and tried proging it!

Did the reflow on a hotplate

![image.png](https://cdn.hackclub.com/019eb5d5-137f-7734-a47e-374a6717fd2a/image.png)

Played with it a tiny bit...

# 2026-06-11: Finished BOM

**Total time spent: 2.2 hours**

![image.png](https://cdn.hackclub.com/019eb5c9-8e9e-728d-bd92-456eafdee350/image.png)

Finished the BOM! Bom optimized this to use the least components.

Pretty proud of the job I did for this

Finished the BOM! Bom optimized the use the least components.

Pretty proud of the job I did for this

![image.png](https://cdn.hackclub.com/019eb5ca-4d74-77a1-9315-d335ed129cf9/image.png)

Got the grant!

# 2026-06-11: Finished PCB 

**Total time spent: 8.9 hours**

![image.png](https://cdn.hackclub.com/019eb5cb-380f-7fbb-878a-43a7e9a1c568/image.png)

Added a light sensor - chose one that satisfied reqs and was cheap

![image.png](https://cdn.hackclub.com/019eb5c7-e969-7678-8e1a-c7d2cda1ac6c/image.png)

Finished the PCB - re-routed it a few times to ensure the lowest size and smallest form factor.

![image.png](https://cdn.hackclub.com/019eb5c8-0453-75af-8161-8f9665cec1c2/image.png)

Used these types of holes to remove mechanical stress from solder joints

![image.png](https://cdn.hackclub.com/019eb5c8-20ea-7353-b398-1a9ad1040e08/image.png)

![image.png](https://cdn.hackclub.com/019eb5c8-2e4a-713a-906c-0e76ce35fc53/image.png)
Also got a quote for the PCB and stencil

![image.png](https://cdn.hackclub.com/019eb5c8-65c3-7fa6-aafd-ce5faa917078/image.png)

Also made custom footpritns

![image.png](https://cdn.hackclub.com/019eb5c8-9ee6-728a-a413-3053c7131b4e/image.png)

![image.png](https://cdn.hackclub.com/019eb5c9-1639-7dc6-aba3-7b037cbd72a8/image.png)

A custom symbol for the v006 was also made but not used

# 2026-06-11: Research, discussions and end schematic 

**Total time spent: 7.9 hours**

![image.png](https://cdn.hackclub.com/019eb5c6-7e6d-7ff3-a5c9-b59158b41c64/image.png)

Had a big very fancy and lengthy discussion with people on a discord server about the project! Also discussed scope even more

![image.png](https://cdn.hackclub.com/019eb5c6-a144-7f7c-a42d-bd3b5c2fa667/image.png)

Also did a ton of research on old xenon flashes, including pulling up designs from repair shops and forums


![image.png](https://cdn.hackclub.com/019eb5c6-c0c6-71d5-8d7b-22b26a111f65/image.png)

![image.png](https://cdn.hackclub.com/019eb5c6-cf7e-7bed-ba7f-ca32ba2889ed/image.png)

Also looked into alternative light recieving sources, and added a 2nd impl.![image.png](https://cdn.hackclub.com/019eb5c6-e60d-7cb2-a791-6e23882b41fa/image.png)

Made all this circuit in the time too

![image.png](https://cdn.hackclub.com/019eb5c7-0ebc-700c-843a-2e9d8c942e50/image.png)
Mainly implemented high-voltage switching that can be controled by 3.3v mcu

![image.png](https://cdn.hackclub.com/019eb5c7-90e4-7ef2-b621-45b4f3a1af71/image.png)

The 7seg also got multiplexed

# 2026-06-11: Version 1 of the schematic 

**Total time spent: 6.5 hours**

![image.png](https://cdn.hackclub.com/019eb5c4-f498-72e9-aaba-136540c1e8f8/image.png)

Bom optimized the hell out of this and sqeezed as much money out of it

![image.png](https://cdn.hackclub.com/019eb5c5-0dd7-7641-92ad-c81862a83d69/image.png)

And asked a ton of follow-up questions about the project

![image.png](https://cdn.hackclub.com/019eb5c5-d804-721a-b8d5-ffba81331cf3/image.png)

Read a bit of AoE to get the designs right

![image.png](https://cdn.hackclub.com/019eb5c6-1672-72f9-a944-782a285ea5fd/image.png)

Using built-in opa as amplifier circuit

# 2026-06-11: Learnign about project requirements 

**Total time spent: 3.9 hours**

![image.png](https://cdn.hackclub.com/019eb5c4-109f-7b25-951f-4a2d65054cc4/image.png)
![image.png](https://cdn.hackclub.com/019eb5c4-22b2-7891-9564-619eed30b360/image.png)
![image.png](https://cdn.hackclub.com/019eb5c4-3441-7d83-bc6a-9835bc7ba6d2/image.png)
![image.png](https://cdn.hackclub.com/019eb5c4-4f6e-7507-9e30-2f562e9adbcc/image.png)

Had big discussions w/ HQ member about the requirements and scope of the project, refer to the images

![image.png](https://cdn.hackclub.com/019eb5c4-ad5b-7dd4-8f2b-8252e797579a/image.png)

Did research on what was needed etc

