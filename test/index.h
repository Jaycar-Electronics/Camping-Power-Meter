<!DOCTYPE HTML>
<html>
		<head>
				<script>
window.onload = function () {
		var dps = [0]; // dataPoints
		var dataLength = 20; // number of dataPoints visible at any point
		var chart = new CanvasJS.Chart("chartContainer", {
				title :{
						text: "Current Meter"
				},
				axisY: {
						includeZero: false
				},
				data: [{
						type: "line",
						dataPoints: dps
				}]
		});
		setInterval(function(){asyncRead('/current_reading',updateChart)}, 100);
		function asyncRead(url, func){ //read data from ESP8266
				var xhttp = new XMLHttpRequest();
				xhttp.onreadystatechange = function(){
						if(this.readyState == 4 && this.status == 200){ func.apply(xhttp); }
				};
				xhttp.open(\"GET\", url, true);
				xhttp.send();
		}

		function updateChart () {   //update Chart with data
				count = this.responseText;
				dps.push(count);
				if (dps.length > dataLength) { dps.shift(); }
				chart.render();
		};
}
				</script>
		</head>
		<body>
				<div id="chartContainer" style="height: 300px; width: 100%;"></div>
				<script src="https://canvasjs.com/assets/script/canvasjs.min.js"></script>
		</body>
</html>