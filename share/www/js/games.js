
class Game extends Obj {
	constructor(arg = false)
	{
		super("/api/game", arg);
	}
    
    icon(){
        return this.data.icon;
    }

    title(){
        return this.data.title;
    }
    
    brief(){
        return this.data.brief;
    }
};

function toGames(data) {
    var ret = [];
    for( const x of data)
        ret.push(new Game(x));
    return ret;
}

