/*
This script runs on your phone via the ESP8266
 */
window.onload = function(){
	//timescale in seconds
	var time_scale = 10;
	//resolution, (amount of data to hold over the above X seconds)
	var data_len = 50;
	//---
	var t_res = time_scale / data_len;
	var datapoints = [];
	for(i = 0; i < data_len; i++){datapoints.push(0);}

	var reading_span = document.getElementById('reading');
	var average_span = document.getElementById('average');
	var seconds_span = document.getElementById('sec');
	seconds_span.innerText = time_scale;
	var log = document.getElementById('log');
	var canvas = document.getElementById('chart');
	var ctx = canvas.getContext("2d");
	var avg = 0;

	function asyncRead(url, func){ //read data from ESP8266
			var xhttp = new XMLHttpRequest();
			xhttp.onreadystatechange = function(){
					if(this.readyState == 4 && this.status == 200){ func(xhttp.responseText); }
			};
			xhttp.open("GET", url, true);
			xhttp.send();
	}

	function updateData(data){
		datapoints.push(parseFloat(data));
		datapoints.shift();
		var sum = 0;
		for( var i = 0; i < datapoints.length; i++ ){
			sum += parseInt( datapoints[i], 10 ); //don't forget to add the base
		}
		avg = sum/datapoints.length;

		updatePage(data);
	}

	function updatePage(data){

		reading_span.innerText = parseFloat(data).toFixed(3);
		average_span.innerText = avg;
		var dist = canvas.width / data_len;
		ctx.clearRect(0, 0, canvas.width, canvas.height);
		ctx.strokeStyle = "#0000FF";
		ctx.moveTo(0, 0);
		ctx.beginPath();
		p = 0;

		datapoints.forEach(function(value){
			
			ctx.lineTo(p,canvas.height * (1 - value));
			p += dist;
		});
		ctx.stroke();
	}

	setInterval(function(){asyncRead('current',updateData)},t_res * 1000);

	var button = document.getElementById('capture');
	button.onclick = function(){
		var d = new Date();
		var dateString =
			("0" + d.getHours()).slice(-2) + ":" +
			("0" + d.getMinutes()).slice(-2) + ":" +
			("0" + d.getSeconds()).slice(-2) + " " +
			d.getFullYear() + "/" +
			("0" + (d.getMonth()+1)).slice(-2) + "/" +
			("0" + d.getDate()).slice(-2);

		var innerText = 'Reading: ' + datapoints.slice(-1)[0].toFixed(3) + ' at ' + dateString + "\n" ;
		log.value = innerText + log.value;
		//log.insertBefore(p,log.firstChild);
	};

	var checkbox = document.getElementById('checkbox');
	checkbox.onclick = function(){
		if (checkbox.checked == true){
			checkbox_interval = setInterval(function(){button.onclick();},500);
		}
		else{
			console.log('UNCHECKED');
			clearInterval(checkbox_interval);
		}
	}

	var copier = document.getElementById('copier');
	copier.onclick = function(){
		log.select();
		document.execCommand('copy');
	}
}
