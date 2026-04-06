// ============================================
// SHIROTA: BLOOD POLARITY
// A fighting game based on "Please Put Them On, Takamine-san"
// ============================================

// Game constants
const GRAVITY = 0.5;
const JUMP_FORCE = -14;
const PLAYER_SPEED = 6;
const ENEMY_SPEED = 4;
const AIR_RESISTANCE = 0.95;

// Polarity system
const POLARITY = {
    LIGHT: 'light',
    DARK: 'dark'
};

// Canvas setup
const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');
const playerHpBar = document.getElementById('playerHpBar');
const enemyHpBar = document.getElementById('enemyHpBar');
const playerPolarity = document.getElementById('playerPolarity');
const enemyPolarity = document.getElementById('enemyPolarity');

// Input state
const keys = {};

// Game state
let wins = 0;
let gameOver = false;
let gameTime = 0;

// Particle system
const particles = [];
class Particle {
    constructor(x, y, color, velocityX, velocityY, size, lifetime) {
        this.x = x;
        this.y = y;
        this.color = color;
        this.velocityX = velocityX;
        this.velocityY = velocityY;
        this.size = size;
        this.lifetime = lifetime;
        this.maxLifetime = lifetime;
        this.alpha = 1;
    }

    update() {
        this.x += this.velocityX;
        this.y += this.velocityY;
        this.lifetime--;
        this.alpha = this.lifetime / this.maxLifetime;
        this.velocityY += 0.1; // gravity
    }

    draw() {
        ctx.save();
        ctx.globalAlpha = this.alpha;
        ctx.fillStyle = this.color;
        ctx.beginPath();
        ctx.arc(this.x, this.y, this.size, 0, Math.PI * 2);
        ctx.fill();
        ctx.restore();
    }
}

// Create particles explosion
function createParticles(x, y, color, count = 10) {
    for (let i = 0; i < count; i++) {
        const angle = Math.random() * Math.PI * 2;
        const speed = 2 + Math.random() * 4;
        particles.push(new Particle(
            x, y,
            color,
            Math.cos(angle) * speed,
            Math.sin(angle) * speed - 2,
            2 + Math.random() * 3,
            30 + Math.random() * 20
        ));
    }
}

// Base Fighter class
class Fighter {
    constructor(x, y, isPlayer, name, mainColor, polarity) {
        this.x = x;
        this.y = y;
        this.width = 50;
        this.height = 80;
        this.isPlayer = isPlayer;
        this.name = name;
        this.mainColor = mainColor;
        this.polarity = polarity;
        this.velocityX = 0;
        this.velocityY = 0;
        this.isGrounded = false;
        this.health = 100;
        this.maxHealth = 100;
        this.isAttacking = false;
        this.attackTimer = 0;
        this.cooldownTimer = 0;
        this.facing = isPlayer ? 1 : -1;
        this.comboCount = 0;
        this.comboTimer = 0;
        this.specialMeter = 0;
        this.maxSpecial = 100;
        this.isBlocking = false;
        this.hitStun = 0;
        this.airBorne = false;
        this.attackType = 'normal'; // normal, lightSpecial, darkSpecial
        this.auraRadius = 0;
        this.dashCooldown = 0;
        this.dashDirection = 0;
    }

    update(opponent) {
        if (this.hitStun > 0) {
            this.hitStun--;
            this.velocityX *= 0.8;
            this.velocityY += GRAVITY;
            this.x += this.velocityX;
            this.y += this.velocityY;
            return;
        }

        // Apply gravity
        if (!this.isGrounded) {
            this.velocityY += GRAVITY;
        }

        // Apply dash momentum
        if (this.dashCooldown > 0) {
            this.dashCooldown--;
            this.velocityX *= 0.9;
        }

        // Update position
        this.x += this.velocityX;
        this.y += this.velocityY;

        // Ground collision
        const groundY = canvas.height - this.height - 20;
        if (this.y >= groundY) {
            this.y = groundY;
            this.velocityY = 0;
            this.isGrounded = true;
            this.airBorne = false;
        } else {
            this.isGrounded = false;
        }

        // Wall collision
        if (this.x < 0) this.x = 0;
        if (this.x + this.width > canvas.width) this.x = canvas.width - this.width;

        // Attack timer
        if (this.attackTimer > 0) {
            this.attackTimer--;
            if (this.attackTimer === 0) {
                this.isAttacking = false;
                this.attackType = 'normal';
            }
        }

        // Cooldown timer
        if (this.cooldownTimer > 0) {
            this.cooldownTimer--;
        }

        // Combo timer
        if (this.comboTimer > 0) {
            this.comboTimer--;
            if (this.comboTimer === 0) {
                this.comboCount = 0;
            }
        }

        // Aura animation
        if (this.auraRadius > 0) {
            this.auraRadius *= 0.95;
            if (this.auraRadius < 1) this.auraRadius = 0;
        }
    }

    draw() {
        const polarityColor = this.polarity === POLARITY.LIGHT ?
            `rgba(136, 204, 255, 0.8)` :
            `rgba(255, 136, 204, 0.8)`;

        // Draw aura when attacking or special
        if (this.auraRadius > 0) {
            ctx.save();
            const gradient = ctx.createRadialGradient(
                this.x + this.width/2, this.y + this.height/2, 0,
                this.x + this.width/2, this.y + this.height/2, this.auraRadius
            );
            gradient.addColorStop(0, polarityColor);
            gradient.addColorStop(1, 'transparent');
            ctx.fillStyle = gradient;
            ctx.beginPath();
            ctx.arc(this.x + this.width/2, this.y + this.height/2, this.auraRadius, 0, Math.PI * 2);
            ctx.fill();
            ctx.restore();
        }

        // Draw body with gradient
        const gradient = ctx.createLinearGradient(this.x, this.y, this.x, this.y + this.height);
        gradient.addColorStop(0, this.mainColor);
        gradient.addColorStop(1, this.shadeColor(this.mainColor, -30));

        ctx.fillStyle = gradient;
        ctx.fillRect(this.x, this.y, this.width, this.height);

        // Draw polarity symbol on chest
        ctx.save();
        ctx.translate(this.x + this.width/2, this.y + 25);
        ctx.fillStyle = this.polarity === POLARITY.LIGHT ? '#88ccff' : '#ff88cc';
        ctx.beginPath();
        if (this.polarity === POLARITY.LIGHT) {
            // Light: circle (sun/moon)
            ctx.arc(0, 0, 8, 0, Math.PI * 2);
        } else {
            // Dark: crescent moon shape
            ctx.arc(3, 0, 8, Math.PI * 1.2, Math.PI * 1.8);
        }
        ctx.fill();
        ctx.restore();

        // Draw head
        const headY = this.y - 20;
        const headSize = 25;

        // Face
        ctx.fillStyle = this.isPlayer ? '#ffd4a3' : '#e8c4a8';
        ctx.fillRect(this.x + 10, headY, 30, 25);

        // Hair (anime style spiky)
        ctx.fillStyle = this.isPlayer ? '#2a2a4a' : '#6a2a4a';
        for (let i = 0; i < 5; i++) {
            const spikeX = this.x + 10 + i * 6;
            const spikeY = headY - 5 - Math.random() * 3;
            ctx.fillRect(spikeX, spikeY, 4, 8);
        }

        // Eyes
        const eyeY = headY + 10;
        const leftEyeX = this.x + 15;
        const rightEyeX = this.x + 30;

        if (this.isAttacking) {
            // Intense eyes during attack
            ctx.fillStyle = '#fff';
            ctx.fillRect(leftEyeX, eyeY, 5, 3);
            ctx.fillRect(rightEyeX, eyeY, 5, 3);
        } else if (this.hitStun > 0) {
            // Dizzy eyes
            ctx.fillStyle = '#fff';
            ctx.beginPath();
            ctx.arc(leftEyeX + 2, eyeY + 1, 3, 0, Math.PI * 2);
            ctx.arc(rightEyeX + 2, eyeY + 1, 3, 0, Math.PI * 2);
            ctx.fill();
        } else {
            // Normal eyes
            ctx.fillStyle = '#fff';
            ctx.fillRect(leftEyeX, eyeY, 6, 6);
            ctx.fillRect(rightEyeX, eyeY, 6, 6);
            ctx.fillStyle = '#000';
            ctx.fillRect(leftEyeX + (this.facing === 1 ? 2 : 0), eyeY + 2, 3, 3);
            ctx.fillRect(rightEyeX + (this.facing === 1 ? 2 : 0), eyeY + 2, 3, 3);
        }

        // Draw blocking shield
        if (this.isBlocking) {
            ctx.strokeStyle = this.polarity === POLARITY.LIGHT ?
                'rgba(136, 204, 255, 0.6)' :
                'rgba(255, 136, 204, 0.6)';
            ctx.lineWidth = 3;
            ctx.beginPath();
            ctx.arc(this.x + this.width/2, this.y + this.height/2, 40, 0, Math.PI * 2);
            ctx.stroke();
        }

        // Draw attack indicator
        if (this.isAttacking) {
            const attackX = this.x + (this.facing === 1 ? this.width : -30);
            const attackColor = this.attackType === 'lightSpecial' ? '#88ccff' :
                               this.attackType === 'darkSpecial' ? '#ff88cc' :
                               this.polarity === POLARITY.LIGHT ? '#aaddff' : '#ffaadd';

            ctx.save();
            ctx.globalAlpha = 0.6 + 0.4 * Math.sin(gameTime * 0.3);
            ctx.fillStyle = attackColor;
            ctx.beginPath();
            ctx.arc(attackX + 15, this.y + this.height/2,
                   this.attackType === 'lightSpecial' || this.attackType === 'darkSpecial' ? 25 : 15,
                   0, Math.PI * 2);
            ctx.fill();
            ctx.restore();
        }

        // Draw special meter
        const meterWidth = 30;
        const meterHeight = 4;
        const meterX = this.x + (this.width - meterWidth) / 2;
        const meterY = this.y - 30;
        ctx.fillStyle = '#333';
        ctx.fillRect(meterX, meterY, meterWidth, meterHeight);
        const specialPercent = this.specialMeter / this.maxSpecial;
        const specialColor = specialPercent >= 1 ? '#ff0' : this.polarity === POLARITY.LIGHT ? '#88ccff' : '#ff88cc';
        ctx.fillStyle = specialColor;
        ctx.fillRect(meterX, meterY, meterWidth * specialPercent, meterHeight);
    }

    shadeColor(color, percent) {
        const num = parseInt(color.replace("#", ""), 16);
        const amt = Math.round(2.55 * percent);
        const R = (num >> 16) + amt;
        const G = (num >> 8 & 0x00FF) + amt;
        const B = (num & 0x0000FF) + amt;
        return "#" + (0x1000000 +
            (R < 255 ? R < 1 ? 0 : R : 255) * 0x10000 +
            (G < 255 ? G < 1 ? 0 : G : 255) * 0x100 +
            (B < 255 ? B < 1 ? 0 : B : 255)
        ).toString(16).slice(1);
    }

    attack(type = 'normal') {
        if (this.cooldownTimer > 0 || this.hitStun > 0) return false;

        this.isAttacking = true;
        this.attackType = type;
        this.attackTimer = type === 'normal' ? 15 : 25;

        if (type === 'lightSpecial' || type === 'darkSpecial') {
            this.cooldownTimer = 40;
            this.specialMeter = 0;
            this.auraRadius = 80;
            return 25; // special damage
        }

        this.cooldownTimer = 20;
        this.comboTimer = 30;
        this.comboCount++;
        return 5 + Math.floor(this.comboCount * 1.5); // scaling combo damage
    }

    takeDamage(amount, attacker) {
        if (this.isBlocking) {
            amount *= 0.2; // 80% damage reduction
            this.specialMeter = Math.min(this.maxSpecial, this.specialMeter + 20);
        }

        this.health -= amount;
        this.hitStun = 20;

        // Knockback
        const knockback = 8;
        this.velocityX = (this.x - attacker.x) > 0 ? knockback : -knockback;
        if (!this.isGrounded) {
            this.velocityY = -5;
            this.airBorne = true;
        }

        // Blood particles
        const bloodColor = this.isPlayer ? '#ff4444' : '#ff6688';
        createParticles(this.x + this.width/2, this.y + 30, bloodColor, 8);

        if (this.health < 0) this.health = 0;
        return this.health <= 0;
    }

    block(blocking) {
        if (this.cooldownTimer > 0) return;
        this.isBlocking = blocking;
        if (blocking) {
            this.velocityX *= 0.5;
        }
    }

    changePolarity() {
        this.polarity = this.polarity === POLARITY.LIGHT ? POLARITY.DARK : POLARITY.LIGHT;
        // Visual effect
        this.auraRadius = 50;
        const particlesCount = 20;
        const particleColor = this.polarity === POLARITY.LIGHT ?
            'rgba(136, 204, 255, 0.8)' :
            'rgba(255, 136, 204, 0.8)';
        for (let i = 0; i < particlesCount; i++) {
            const angle = Math.random() * Math.PI * 2;
            const speed = 3 + Math.random() * 3;
            particles.push(new Particle(
                this.x + this.width/2, this.y + this.height/2,
                particleColor,
                Math.cos(angle) * speed,
                Math.sin(angle) * speed,
                4 + Math.random() * 3,
                40 + Math.random() * 20
            ));
        }
    }

    jump() {
        if (this.isGrounded && this.hitStun === 0) {
            this.velocityY = JUMP_FORCE;
            this.isGrounded = false;
            this.airBorne = true;
        }
    }

    move(direction) {
        if (this.hitStun > 0) return;
        const speed = this.isGrounded ? PLAYER_SPEED : PLAYER_SPEED * 0.7;
        this.velocityX = direction * speed;
        this.facing = direction;
    }

    dash(direction) {
        if (this.dashCooldown > 0 || this.hitStun > 0) return;
        this.dashDirection = direction;
        this.velocityX = direction * 20;
        this.dashCooldown = 30;
        this.airBorne = true;
        this.auraRadius = 60;
    }
}

// Player class
class Player extends Fighter {
    constructor() {
        super(150, 300, true, 'Koushi Shirota', '#4af', POLARITY.LIGHT);
    }

    handleInput() {
        // Movement
        if (keys['a'] || keys['A'] || keys['ArrowLeft']) {
            this.move(-1);
        } else if (keys['d'] || keys['D'] || keys['ArrowRight']) {
            this.move(1);
        } else {
            this.velocityX *= 0.8;
        }

        // Jump
        if (keys['w'] || keys['W'] || keys['ArrowUp'] || keys[' ']) {
            this.jump();
        }

        // Block
        if (keys['s'] || keys['S'] || keys['ArrowDown']) {
            this.block(true);
        } else {
            this.block(false);
        }

        // Attacks
        if (keys[' '] || keys['z'] || keys['Z']) {
            const damage = this.attack('normal');
            if (damage) {
                createParticles(
                    this.x + (this.facing === 1 ? this.width : -10),
                    this.y + this.height/2,
                    this.polarity === POLARITY.LIGHT ? '#88ccff' : '#ff88cc',
                    5
                );
            }
        }

        // Special attacks
        if ((keys['q'] || keys['Q']) && this.specialMeter >= this.maxSpecial) {
            const damage = this.attack('lightSpecial');
            if (damage) {
                createParticles(
                    this.x + (this.facing === 1 ? this.width : -20),
                    this.y + this.height/2,
                    '#88ccff',
                    15
                );
            }
        }
        if ((keys['e'] || keys['E']) && this.specialMeter >= this.maxSpecial) {
            const damage = this.attack('darkSpecial');
            if (damage) {
                createParticles(
                    this.x + (this.facing === 1 ? this.width : -20),
                    this.y + this.height/2,
                    '#ff88cc',
                    15
                );
            }
        }

        // Polarity change
        if (keys['Shift']) {
            this.changePolarity();
        }

        // Dash
        if (keys['v'] || keys['V']) {
            this.dash(this.facing);
        }
    }

    draw() {
        super.draw();

        // Draw name above
        ctx.fillStyle = '#4af';
        ctx.font = 'bold 12px Arial';
        ctx.textAlign = 'center';
        ctx.fillText(this.name, this.x + this.width/2, this.y - 35);
    }
}

// Enemy AI class
class Enemy extends Fighter {
    constructor() {
        super(750, 300, false, 'Takane Takamine', '#f44', POLARITY.DARK);
        this.aiState = 'approach';
        this.aiTimer = 0;
        this.aggroRange = 300;
        this.attackRange = 80;
    }

    updateAI(player, playerAttacking) {
        if (this.hitStun > 0) return;

        const dx = player.x - this.x;
        const distance = Math.abs(dx);
        const dy = player.y - this.y;

        this.aiTimer--;

        // Decide action based on state
        if (this.aiTimer <= 0) {
            if (distance > this.aggroRange) {
                this.aiState = 'approach';
                this.aiTimer = 30;
            } else if (distance > this.attackRange) {
                this.aiState = Math.random() < 0.7 ? 'approach' : 'jump';
                this.aiTimer = 40;
            } else {
                // Close range - attack or block
                if (playerAttacking && Math.random() < 0.6) {
                    this.aiState = 'block';
                    this.aiTimer = 20;
                } else {
                    this.aiState = Math.random() < 0.5 ? 'attack' : 'special';
                    this.aiTimer = 15;
                }
            }

            // Sometimes change polarity
            if (Math.random() < 0.05) {
                this.changePolarity();
            }
        }

        // Execute AI state
        switch (this.aiState) {
            case 'approach':
                if (distance > 60) {
                    this.move(dx > 0 ? 1 : -1);
                } else {
                    this.velocityX *= 0.9;
                }
                break;

            case 'attack':
                if (distance <= this.attackRange) {
                    const damage = this.attack('normal');
                    if (damage) {
                        createParticles(
                            this.x + (this.facing === 1 ? this.width : -10),
                            this.y + this.height/2,
                            '#ff88cc',
                            5
                        );
                    }
                }
                break;

            case 'special':
                if (this.specialMeter >= this.maxSpecial && distance <= this.attackRange + 30) {
                    const type = Math.random() < 0.5 ? 'lightSpecial' : 'darkSpecial';
                    const damage = this.attack(type);
                    if (damage) {
                        createParticles(
                            this.x + (this.facing === 1 ? this.width : -20),
                            this.y + this.height/2,
                            type === 'lightSpecial' ? '#88ccff' : '#ff88cc',
                            15
                        );
                    }
                } else {
                    this.aiState = 'approach';
                    this.aiTimer = 20;
                }
                break;

            case 'block':
                this.block(true);
                break;

            case 'jump':
                if (this.isGrounded) {
                    this.jump();
                }
                break;
        }

        // Stop blocking when not needed
        if (this.aiState !== 'block') {
            this.block(false);
        }
    }

    draw() {
        super.draw();

        // Draw name above
        ctx.fillStyle = '#f44';
        ctx.font = 'bold 12px Arial';
        ctx.textAlign = 'center';
        ctx.fillText(this.name, this.x + this.width/2, this.y - 35);
    }
}

// Collision detection
function checkAttackCollision(attacker, target) {
    if (!attacker.isAttacking || attacker.attackTimer <= 0) return false;

    let reach = 60;
    if (attacker.attackType === 'lightSpecial' || attacker.attackType === 'darkSpecial') {
        reach = 100;
    }

    const ax = attacker.x + (attacker.facing === 1 ? attacker.width : -reach);
    const ay = attacker.y;
    const aw = reach;
    const ah = attacker.height;

    // AABB collision
    if (ax < target.x + target.width &&
        ax + aw > target.x &&
        ay < target.y + target.height &&
        ay + ah > target.y) {
        return true;
    }

    return false;
}

// Reset game
function resetGame() {
    player.health = player.maxHealth;
    enemy.health = enemy.maxHealth;
    player.x = 150;
    player.y = 300;
    enemy.x = 750;
    enemy.y = 300;
    player.velocityX = 0;
    player.velocityY = 0;
    enemy.velocityX = 0;
    enemy.velocityY = 0;
    player.polarity = POLARITY.LIGHT;
    enemy.polarity = POLARITY.DARK;
    player.specialMeter = 0;
    enemy.specialMeter = 0;
    player.comboCount = 0;
    enemy.comboCount = 0;
    gameOver = false;
}

// Initialize
const player = new Player();
const enemy = new Enemy();

// Event listeners
window.addEventListener('keydown', e => {
    keys[e.key] = true;
    if (e.key === ' ' && gameOver) {
        wins++;
        resetGame();
    }
    // Prevent scrolling with arrow keys
    if (['ArrowUp', 'ArrowDown', 'ArrowLeft', 'ArrowRight', ' '].includes(e.key)) {
        e.preventDefault();
    }
});
window.addEventListener('keyup', e => keys[e.key] = false);

// Draw background
function drawBackground() {
    // Sky gradient
    const skyGradient = ctx.createLinearGradient(0, 0, 0, canvas.height);
    skyGradient.addColorStop(0, '#0a0a1a');
    skyGradient.addColorStop(0.5, '#1a0510');
    skyGradient.addColorStop(1, '#1a0a0a');
    ctx.fillStyle = skyGradient;
    ctx.fillRect(0, 0, canvas.width, canvas.height);

    // Stars
    ctx.fillStyle = 'rgba(255, 255, 255, 0.5)';
    for (let i = 0; i < 50; i++) {
        const x = (i * 19 + gameTime * 0.1) % canvas.width;
        const y = (i * 7) % (canvas.height - 100);
        const size = (Math.sin(gameTime * 0.2 + i) + 1) * 0.5;
        ctx.fillRect(x, y, size, size);
    }

    // Moon
    ctx.fillStyle = 'rgba(255, 255, 255, 0.1)';
    ctx.beginPath();
    ctx.arc(canvas.width - 100, 80, 40, 0, Math.PI * 2);
    ctx.fill();

    // Ground
    const groundGradient = ctx.createLinearGradient(0, canvas.height - 40, 0, canvas.height);
    groundGradient.addColorStop(0, '#2a1a1a');
    groundGradient.addColorStop(1, '#0a0505');
    ctx.fillStyle = groundGradient;
    ctx.fillRect(0, canvas.height - 20, canvas.width, 20);

    // Platform
    ctx.fillStyle = 'rgba(100, 50, 50, 0.3)';
    ctx.fillRect(200, canvas.height - 120, 600, 10);
}

// Game loop
function gameLoop() {
    gameTime++;

    if (gameOver) {
        requestAnimationFrame(gameLoop);

        // Darken and draw game over
        drawBackground();
        player.draw();
        enemy.draw();

        ctx.fillStyle = 'rgba(0, 0, 0, 0.7)';
        ctx.fillRect(0, 0, canvas.width, canvas.height);

        ctx.fillStyle = '#fff';
        ctx.font = 'bold 56px Arial';
        ctx.textAlign = 'center';
        ctx.textBaseline = 'middle';
        ctx.fillText('VICTORY!', canvas.width/2, canvas.height/2 - 40);

        ctx.font = '24px Arial';
        ctx.fillText('Press SPACE to continue', canvas.width/2, canvas.height/2 + 40);

        return;
    }

    // Update
    if (!gameOver) {
        // Handle player input
        if (!player.isPlayer) {
            // AI control for player (for demo)
        } else {
            player.handleInput();
        }

        // Enemy AI
        enemy.updateAI(player, player.isAttacking);

        // Update fighters
        player.update(enemy);
        enemy.update(player);

        // Player attack input (additional trigger)
        if (keys[' '] || keys['z'] || keys['Z'] || keys['Enter']) {
            const damage = player.attack('normal');
            if (damage) {
                createParticles(
                    player.x + (player.facing === 1 ? player.width : -10),
                    player.y + player.height/2,
                    player.polarity === POLARITY.LIGHT ? '#88ccff' : '#ff88cc',
                    5
                );
            }
        }

        // Special attacks input
        if ((keys['q'] || keys['Q']) && player.specialMeter >= player.maxSpecial) {
            const damage = player.attack('lightSpecial');
            if (damage) {
                createParticles(
                    player.x + (player.facing === 1 ? player.width : -20),
                    player.y + player.height/2,
                    '#88ccff',
                    15
                );
            }
        }
        if ((keys['e'] || keys['E']) && player.specialMeter >= player.maxSpecial) {
            const damage = player.attack('darkSpecial');
            if (damage) {
                createParticles(
                    player.x + (player.facing === 1 ? player.width : -20),
                    player.y + player.height/2,
                    '#ff88cc',
                    15
                );
            }
        }

        // Check attack collisions (player hitting enemy)
        if (checkAttackCollision(player, enemy)) {
            const damage = player.attackType === 'normal' ?
                5 + Math.floor(player.comboCount * 1.5) :
                (player.attackType === 'lightSpecial' || player.attackType === 'darkSpecial') ? 25 : 5;
            enemy.takeDamage(damage, player);
            player.specialMeter = Math.min(player.maxSpecial, player.specialMeter + 15);
        }

        // Check attack collisions (enemy hitting player)
        if (checkAttackCollision(enemy, player)) {
            const damage = enemy.attackType === 'normal' ? 5 : 25;
            player.takeDamage(damage, enemy);
            if (player.health <= 0) {
                gameOver = true;
            }
        }

        // Check win/lose
        if (enemy.health <= 0) {
            wins++;
            gameOver = true;
        }

        // Update particles
        for (let i = particles.length - 1; i >= 0; i--) {
            particles[i].update();
            if (particles[i].lifetime <= 0) {
                particles.splice(i, 1);
            }
        }

        // Update UI
        playerHpBar.style.width = `${(player.health / player.maxHealth) * 100}%`;
        enemyHpBar.style.width = `${(enemy.health / enemy.maxHealth) * 100}%`;
        playerPolarity.textContent = player.polarity.toUpperCase();
        playerPolarity.className = `polarity-indicator polarity-${player.polarity}`;
        enemyPolarity.textContent = enemy.polarity.toUpperCase();
        enemyPolarity.className = `polarity-indicator polarity-${enemy.polarity}`;
    }

    // Draw
    drawBackground();

    // Draw particles behind fighters
    particles.forEach(p => p.draw());

    // Draw fighters
    player.draw();
    enemy.draw();

    requestAnimationFrame(gameLoop);
}

// Start game
resetGame();
gameLoop();
