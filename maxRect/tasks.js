let MaxRectsPacker = require("maxrects-packer").MaxRectsPacker;
let Rectangle = require("maxrects-packer").Rectangle;

// - trudność zadania  (0.5, 0.75, 0.9)
// - różnorodność pudełek (5, 10, 20)
// - przedział liczby pudełek danego typu(10-20, 10-100, 10-1000)

let difficulties = [0.5, 0.75, 1];
let variants = [5, 10, 20];
const sizes = [
  [10, 20],
  [10, 100],
  [10, 1000],
];

let itemsCount = [2, 50];

function getRandomInt(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min)) + min;
}

const generateTasks = () => {
  const tasks = [];

  difficulties.forEach((difficulty) =>
    variants.forEach((variant) =>
      sizes.forEach((size) => {
        let paletteSize = 0;

        const input = [];

        for (let i = 0; i < variant; i++) {
          const count = getRandomInt(itemsCount[0], itemsCount[1]);
          const width = getRandomInt(size[0], size[1]);
          const height = getRandomInt(size[0], size[1]);

          paletteSize += width * height * count;
          input.push([width, height, count]);
        }

        paletteSize /= difficulty;

        tasks.push([
          input,
          [
            Math.ceil(Math.sqrt(paletteSize)),
            Math.ceil(Math.sqrt(paletteSize)),
          ],
        ]);
      })
    )
  );

  return tasks;
};

// const tasks = generateTasks();

// for(let j = 0; j < count; j++){
//     const rectangle = new Rectangle(width, height);
//     paletteSize += width * height;
//     input.push(rectangle);
// }

// Convert the array to a JSON string

// console.log(tasks[0][0]);

// const items_c = JSON.stringify(
//   tasks.map((task, taskIndex) => {
//     return `{${task[0].map((item) => {
//       return `{{${item[0]}, ${item[1]}}, ${item[2]}}`;
//     })}}`;
//   })
// );

// const paletteSizes_c = JSON.stringify(
//   tasks.map((task) => `{${task[1][0]}, ${task[1][1]}}`)
// );

// const items = JSON.stringify(tasks.map((task) => task[0]));
// const paletteSizes = JSON.stringify(tasks.map((task) => task[1]));

// // Write the JSON string to a file
// fs.writeFile("items_c.json", items_c, (err) => {
//   if (err) {
//     console.error("Error writing file:", err);
//     return;
//   }
//   console.log("Array saved as JSON file.");
// });

// fs.writeFile("paletteSizes_c.json", paletteSizes_c, (err) => {
//   if (err) {
//     console.error("Error writing file:", err);
//     return;
//   }
//   console.log("Array saved as JSON file.");
// });

// fs.writeFile("items.json", items, (err) => {
//   if (err) {
//     console.error("Error writing file:", err);
//     return;
//   }
//   console.log("Array saved as JSON file.");
// });

// fs.writeFile("paletteSizes.json", paletteSizes, (err) => {
//   if (err) {
//     console.error("Error writing file:", err);
//     return;
//   }
//   console.log("Array saved as JSON file.");
// });

module.exports = { generateTasks };
