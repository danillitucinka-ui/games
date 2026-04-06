# Картинки для игры - Рекомендации по ассетам

## 🎨 Где скачать подходящие картинки

### 1. **OpenGameArt.org**
- **Персонажи**: Ищите "anime sprite", "pixel character", "chibi sprite"
- **Фоны**: "school background", "city background", "nature scene"
- **Эффекты**: "particle effect", "magic aura", "explosion sprite"

### 2. **Itch.io** (Free Assets)
- **Поиск**: "free sprites", "2d character sprites", "background tiles"
- **Рекомендуемые теги**: pixel art, anime style, fighting game assets

### 3. **Kenney.nl** (Высококачественные бесплатные ассеты)
- **Character Pack**: https://kenney.nl/assets?q=2d-characters
- **Background Pack**: https://kenney.nl/assets?q=background
- **Particle Effects**: https://kenney.nl/assets?q=particles

### 4. **OpenPixelProject**
- Бесплатные пиксель-арт ассеты
- Хороши для спрайтов персонажей

## 📋 Требуемые ассеты

### Персонажи (64x96 PNG с прозрачностью):
```
assets/characters/
├── shirota_normal.png
├── shirota_attack.png
├── shirota_hurt.png
├── takamine_normal.png
├── takamine_tsundere.png
├── takamine_attack.png
├── miki_cheerful.png
├── seiya_mysterious.png
└── kiriko_elegant.png
```

### Фоны (1200x700 PNG):
```
assets/backgrounds/
├── school_rooftop.png
├── classroom.png
├── school_garden.png
├── night_city.png
├── warehouse.png
└── mountain.png
```

### Эффекты и UI:
```
assets/effects/
├── aura_light.png
├── aura_dark.png
├── particle_blood.png
├── particle_spark.png
├── explosion.png
└── hit_effect.png

assets/ui/
├── health_bar.png
├── special_meter.png
├── button_normal.png
└── button_hover.png
```

## 🎵 Звуки и музыка

### Звуковые эффекты (WAV):
```
assets/sounds/
├── attack.wav
├── special.wav
├── hit.wav
├── polarity_change.wav
├── victory.wav
├── defeat.wav
└── dialogue.wav
```

### Музыка (OGG):
```
assets/music/
├── menu_theme.ogg
├── battle_theme.ogg
├── cutscene_theme.ogg
├── victory_theme.ogg
└── defeat_theme.ogg
```

## 🛠️ Как добавить ассеты в игру

### 1. Создать папку assets:
```bash
mkdir -p assets/{characters,backgrounds,effects,ui,sounds,music}
```

### 2. Добавить загрузку в SpriteGenerator:
```cpp
// В SpriteGenerator::generateCharacterSprite добавить:
sf::Texture realTexture;
if (realTexture.loadFromFile("assets/characters/" + characterName + ".png")) {
    textures[characterName + "_real"] = realTexture;
    return realTexture; // Использовать реальное изображение
} else {
    // Использовать процедурную генерацию
    return generateProceduralSprite(characterName, ...);
}
```

### 3. Аналогично для фонов и эффектов

### 4. Для музыки в AudioManager:
```cpp
// Добавить загрузку файлов
audioManager.loadSound("attack_real", "assets/sounds/attack.wav");
audioManager.playBackgroundMusic("assets/music/battle_theme.ogg");
```

## 🎨 Рекомендации по стилю

### Цветовая палитра:
- **Shirota**: Синий (#4AF), светло-синие эффекты
- **Takamine**: Красный (#F44), розовые эффекты
- **Miki**: Желтый (#FFA500), золотые эффекты
- **Seiya**: Фиолетовый (#6A5ACD), энергетические эффекты
- **Kiriko**: Розовый (#FF69B4), элегантные эффекты

### Стиль персонажей:
- Аниме-стиль с большими глазами
- Простая но выразительная анимация
- Цветные волосы и одежда
- Эмоциональные выражения лица

### Фоны:
- Детализированные но не перегруженные
- Аниме-стиль архитектуры
- Атмосферное освещение
- Динамические элементы (облака, листья)

## 📝 Лицензии

При использовании ассетов проверяйте лицензии:
- **CC0/CC-BY**: Можно использовать свободно
- **CC-BY-SA**: Требуется указание автора
- **GPL**: Для open-source проектов

## 🔄 Альтернатива: Создание собственных ассетов

Если не хотите скачивать, используйте:
- **Aseprite** - Создание пиксель-арта
- **GIMP/Photoshop** - Редактирование изображений
- **Audacity** - Создание звуков
- **LMMS** - Создание музыки

Текущая игра работает с процедурно генерируемыми спрайтами, но добавление реальных изображений сделает её ещё красивее! 🎨✨