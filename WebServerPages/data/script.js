

document.addEventListener('DOMContentLoaded', (event) => {
    const ctx = document.getElementById('myCanvas').getContext('2d');

    // Sample data for the line chart
    const data = {
        labels: ['January', 'February', 'March', 'April', 'May', 'June', 'July'],
        datasets: [{
            label: 'Temperature Over Time',
            data: [0, 10, 5, 2, 20, 30, 45],
            fill: false,
            borderColor: 'rgb(75, 192, 192)',
            tension: 0.1
        }]
    };

    // Configuration options for the line chart
    const config = {
        type: 'line',
        data: data,
        options: {
            responsive: true,
            scales: {
                x: {
                    display: true,
                    title: {
                        display: true,
                        text: 'Month'
                    }
                },
                y: {
                    display: true,
                    title: {
                        display: true,
                        text: 'Temperature (°C)'
                    }
                }
            }
        }
    };

    // Create the line chart
    const myChart = new Chart(ctx, config);
});