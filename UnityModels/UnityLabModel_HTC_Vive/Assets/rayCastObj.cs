/**
 * File name: rayCastObj.cs
 * script includes raycast and its interaction with objects
 * By: Opin Patel
 * UIC CS 398 Undergraduate Research
 * 
**/


using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class rayCastObj : MonoBehaviour { // rayCastObj class
	bool doorStatus = false;
	bool glassessOn = false;
	public GameObject explosion;
	public GameObject fire;
	public string disText;
	public Transform target;
	public GameObject onFace;
	public GameObject eyeBleedObject;
	public GameObject spray1ani;
	bool spray1Played = false;
	bool showText = false;
	bool screamPlayed = false;
	public GameObject screamObj;

	// Update is called once per frame
	void Update () {
		RaycastHit hit;      // raycast hit will detect the lazer collision with objects.
		float theDistance;   
		GameObject dDoor;

		// forward vector that will be used with RaycastHit
		Vector3 forward = transform.TransformDirection (Vector3.forward) * 1;
		Debug.DrawRay (transform.position, forward, Color.green);

		// if the lazer hit the door, then we want it to animate using doorOpen animation. 
		if (Physics.Raycast (transform.position, (forward), out hit, 4)) {
			if (hit.collider.gameObject.tag == "door" && doorStatus == false) {
				hit.collider.gameObject.GetComponent<Animation> ().Play ("doorOpen");
				doorStatus = true;
			}
		}

		// same if it hit the safety googles, then play the glassessOnFace animation.
		if (Physics.Raycast (transform.position, (forward), out hit, 4)) {
			if (hit.collider.gameObject.tag == "tableGlass") {
				Destroy (hit.collider.gameObject);
				//print ("wearing glassess");
				onFace.GetComponent<Animation> ().Play ("glassessOnFace");
				glassessOn = true;	
			}

			if (hit.collider.gameObject.tag == "chemicalReactor" || hit.collider.gameObject.tag == "tableExplo" || hit.collider.gameObject.tag == "flask") {
				// Here, we want the explosition particle system to go off.
				Instantiate (explosion, transform.position, transform.rotation);
				if (glassessOn == false) {
					eyeBleedObject.GetComponent<Animation> ().Play ("newAnimation");
					screamObj.GetComponent<AudioSource> ().Play ();
				} 
				if (glassessOn == true && spray1Played == false) {
					print ("spraying animation");
					spray1ani.GetComponent<Animation> ().Play ("spot1new");
					spray1Played = true;
				}
			}
		}
	}
}


//videoPlayer.Play ();
//GameObject camera = GameObject.Find("Quad 9");
// var videoPlayer = camera.AddComponent<UnityEngine.Video.VideoPlayer>();

/**
 *  
 
 
 function OnGUI()
 {
 
     if(showText)
     {
 
     GUI.Label(textArea,"Here is a block of text\nlalalala\nanother line\nI could do this all day!");
 
     }
 
 }
**/